using UnityEngine;

[DefaultExecutionOrder(20)]
public class FurBatcher : MonoBehaviour
{
    public FurCoreMain fur;
    public bool enableInstancing = true;

    [Tooltip("Advanced: convert shells to shared material + MPB for max batching. Turn on ONLY if you've removed per-shell new Material() in FurCoreMain.")]
    public bool convertToSharedMaterialWithMPB = false;

    void Start()
    {
        if (!fur) fur = GetComponent<FurCoreMain>();
        var baseMat = GetComponent<MeshRenderer>()?.sharedMaterial;
        if (baseMat) baseMat.enableInstancing = enableInstancing;

        foreach (var mr in GetComponentsInChildren<MeshRenderer>(true))
        {
            if (mr.sharedMaterial) mr.sharedMaterial.enableInstancing = enableInstancing;
        }

        if (convertToSharedMaterialWithMPB && fur && fur.furMaterial)
        {
            // Replace unique shell materials with a single shared material + MPB values
            var mrs = GetComponentsInChildren<MeshRenderer>(true);
            foreach (var mr in mrs)
            {
                mr.sharedMaterial = fur.furMaterial; // shared
                // Per-renderer overrides via MPB:
                int shellIndex = ParseShellIndex(mr.gameObject.name); // expects "FurShell_X"
                var mpb = new MaterialPropertyBlock();
                mr.GetPropertyBlock(mpb);
                mpb.SetFloat("_ShellIndex", shellIndex);
                mpb.SetFloat("_ShellCount", fur.shellCount);
                mr.SetPropertyBlock(mpb);
                // IMPORTANT: comment out per-shell new Material() in FurCoreMain if you enable this path
            }
        }
    }

    int ParseShellIndex(string goName)
    {
        // "FurShell_12" -> 12 ; base mesh returns 0
        int idx = 0;
        if (goName.StartsWith("FurShell_"))
        {
            var tail = goName.Substring("FurShell_".Length);
            int.TryParse(tail, out idx);
        }
        return idx;
    }
}
