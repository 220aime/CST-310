using System.Collections.Generic;
using UnityEngine;
using Unity.Profiling;

public class FurProfiler : MonoBehaviour
{
    ProfilerRecorder _frameTime, _gpuTime;
    readonly Queue<float> _lastMs = new Queue<float>();
    const int Window = 120; // ~2 seconds at 60fps

    void OnEnable()
    {
        _frameTime = ProfilerRecorder.StartNew(ProfilerCategory.Internal, "FrameTime");
        // GPU time recorder might not exist on all platforms; we'll try Render category:
        try { _gpuTime = ProfilerRecorder.StartNew(ProfilerCategory.Render, "GPU Time"); } catch { }
    }

    void OnDisable()
    {
        _frameTime.Dispose();
        _gpuTime.Dispose();
    }

    void Update()
    {
        if (!_frameTime.Valid) return;
        float ms = _frameTime.LastValue * 1e-6f; // ns -> ms
        _lastMs.Enqueue(ms);
        while (_lastMs.Count > Window) _lastMs.Dequeue();
    }

    void OnGUI()
    {
        if (!_frameTime.Valid) return;
        float[] arr = new float[_lastMs.Count];
        _lastMs.CopyTo(arr, 0);
        System.Array.Sort(arr);
        float p95 = arr.Length > 0 ? arr[Mathf.Clamp(Mathf.FloorToInt(arr.Length * 0.95f) - 1, 0, arr.Length - 1)] : 0f;
        float avg = 0f; foreach (var v in arr) avg += v; avg /= Mathf.Max(1, arr.Length);

        string gpu = (_gpuTime.Valid && _gpuTime.LastValue > 0) ? $"{_gpuTime.LastValue * 1e-6f:F2} ms" : "n/a";
        GUI.Box(new Rect(Screen.width - 210, 12, 200, 90), $"<b>Fur Profiler</b>\nCPU avg: {avg:F2} ms\nCPU p95: {p95:F2} ms\nGPU: {gpu}", new GUIStyle(GUI.skin.box) { richText = true });
    }
}
