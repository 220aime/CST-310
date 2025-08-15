/*AIMME SERGE TUYISHIME
CST-310
ADVANCED SHADER II
8/15/2025
*/
using UnityEngine;

[ExecuteAlways] // works in Edit mode too
[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class FurCoreMain : MonoBehaviour
{
    [Header("Fur Look")]
    [Range(4, 64)] public int shellCount = 32;
    public Material furMaterial;                         // ← drag Fur_Mat here
    [Range(0f, 1f)] public float furLength = 0.30f;
    [Range(0f, 1f)] public float gravity = 0.20f;

    [Header("Wind (optional)")]
    public bool enableWind = true;
    public Vector3 windDirection = new Vector3(0.5f, 0f, 1f);
    public float windStrength = 0.7f;
    public float windFrequency = 2.0f;

    // runtime
    private GameObject[] shells;
    private Material[] shellMats;
    private Mesh srcMesh;
    private int baseLayer;

    void OnEnable()
    {
        srcMesh = GetComponent<MeshFilter>().sharedMesh;
        baseLayer = gameObject.layer;
        TryBuild("OnEnable");
    }

    void OnDisable()
    {
        // Clean editor-time shells when disabling the component (avoid clutter)
        if (!Application.isPlaying) ClearShells();
    }

    void OnDestroy()
    {
        ClearShells();
    }

    void OnValidate()
    {
        // Clamp and rebuild when values change in Inspector
        shellCount = Mathf.Clamp(shellCount, 4, 64);
        if (!isActiveAndEnabled) return;
        TryBuild("OnValidate");
    }

    [ContextMenu("Rebuild Shells Now")]
    public void RebuildNow()
    {
        TryBuild("ContextMenu");
    }

    // ---- core ----
    private void TryBuild(string reason)
    {
        if (furMaterial == null)
        {
            Debug.LogWarning($"[FurCoreMain] Missing Fur Material. Drag your Fur_Mat into the 'Fur Material' slot. ({reason})", this);
            return;
        }
        srcMesh = GetComponent<MeshFilter>().sharedMesh;
        if (srcMesh == null)
        {
            Debug.LogWarning($"[FurCoreMain] No Mesh found on MeshFilter. ({reason})", this);
            return;
        }
        BuildShells();
    }

    private void ClearShells()
    {
        if (shells == null) return;
        foreach (var s in shells)
        {
            if (!s) continue;
            if (Application.isPlaying) Destroy(s);
            else DestroyImmediate(s);
        }
        shells = null;
        shellMats = null;
    }

    public void BuildShells()
    {
        ClearShells();

        shells = new GameObject[shellCount];
        shellMats = new Material[shellCount];

        // base (shell 0) uses the original MeshRenderer
        var baseMR = GetComponent<MeshRenderer>();
        var baseMat = new Material(furMaterial);
        baseMat.SetFloat("_ShellIndex", 0);
        baseMat.SetFloat("_ShellCount", shellCount);
        baseMat.SetFloat("_FurLength", furLength);
        baseMat.SetFloat("_Gravity", gravity);
        baseMat.renderQueue = 3000; // Transparent queue
        baseMR.sharedMaterial = baseMat;

        // shells 1..N-1
        for (int i = 1; i < shellCount; i++)
        {
            var go = new GameObject($"FurShell_{i}");
            go.transform.SetParent(transform, false);
            go.layer = baseLayer;

            var mf = go.AddComponent<MeshFilter>();
            mf.sharedMesh = srcMesh;

            var mr = go.AddComponent<MeshRenderer>();
            var mat = new Material(furMaterial);
            mat.SetFloat("_ShellIndex", i);
            mat.SetFloat("_ShellCount", shellCount);
            mat.SetFloat("_FurLength", furLength);
            mat.SetFloat("_Gravity", gravity);
            mat.renderQueue = 3000 + i;   // per-shell order (3001, 3002, ...)
            mr.sharedMaterial = mat;

            shells[i] = go;
            shellMats[i] = mat;
        }
    }

    void Update()
    {
        // live param updates
        var baseMat = GetComponent<MeshRenderer>().sharedMaterial;
        if (baseMat)
        {
            baseMat.SetFloat("_FurLength", furLength);
            baseMat.SetFloat("_Gravity", gravity);
        }

        if (shellMats != null)
        {
            for (int i = 1; i < shellMats.Length; i++)
            {
                var m = shellMats[i];
                if (!m) continue;
                m.SetFloat("_FurLength", furLength);
                m.SetFloat("_Gravity", gravity);
            }
        }

        if (!enableWind) return;
        float phase = Time.time * windFrequency;
        Vector3 wind = windDirection.normalized * windStrength * (Mathf.Sin(phase) * 0.5f + 0.5f);

        if (baseMat) baseMat.SetVector("_WindDirection", wind);
        if (shellMats != null)
            for (int i = 1; i < shellMats.Length; i++)
                if (shellMats[i]) shellMats[i].SetVector("_WindDirection", wind);
    }
}
