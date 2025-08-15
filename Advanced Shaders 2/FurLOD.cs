using UnityEngine;

[DefaultExecutionOrder(10)]
public class FurLOD : MonoBehaviour
{
    public FurCoreMain fur;                  // drag your Sphere (with FurCoreMain) here
    public Transform targetCamera;           // leave empty to auto-use Camera.main

    [Header("Distances (world units)")]
    public float nearDist = 8f;
    public float midDist = 16f;
    public float farDist = 30f;

    [Header("Shell counts per tier")]
    [Range(1, 64)] public int nearShells = 40;   // 32–48 looks great
    [Range(1, 64)] public int midShells = 16;
    [Range(1, 64)] public int farShells = 8;
    [Range(1, 64)] public int veryFarShells = 4;

    [Header("Features")]
    public bool disablePhysicsBeyondMid = true;    // sets _UsePhysics 0 when mid or worse

    int _appliedShells = -1;

    void Awake()
    {
        if (!fur) fur = GetComponent<FurCoreMain>();
        if (!targetCamera && Camera.main) targetCamera = Camera.main.transform;
    }

    void Update()
    {
        if (!fur) return;
        if (!targetCamera) { if (Camera.main) targetCamera = Camera.main.transform; else return; }

        float d = Vector3.Distance(targetCamera.position, fur.transform.position);
        int targetShells =
            (d <= nearDist) ? nearShells :
            (d <= midDist) ? midShells :
            (d <= farDist) ? farShells : veryFarShells;

        if (targetShells != _appliedShells)
        {
            _appliedShells = Mathf.Clamp(targetShells, 1, 64);
            fur.shellCount = _appliedShells;
            fur.BuildShells(); // rebuild with new shell count

            bool physicsOn = !(disablePhysicsBeyondMid && d > midDist);
            ApplyUsePhysics(physicsOn ? 1f : 0f);
        }
    }

    void ApplyUsePhysics(float onOff)
    {
        // Set _UsePhysics on base + all child shell materials if that uniform exists
        var baseMat = GetComponent<MeshRenderer>()?.sharedMaterial;
        if (baseMat && baseMat.HasFloat("_UsePhysics")) baseMat.SetFloat("_UsePhysics", onOff);

        var children = GetComponentsInChildren<MeshRenderer>(true);
        foreach (var mr in children)
        {
            var m = mr.sharedMaterial;
            if (m && m.HasFloat("_UsePhysics")) m.SetFloat("_UsePhysics", onOff);
        }
    }
}
