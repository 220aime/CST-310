Shader "Fur/Visualizer"
{
    Properties
    {
        _DebugMode ("0=Normals 1=UV 2=ShellH 3=Density", Range(0,3)) = 0
        _ShellIndex ("Shell Index", Float) = 0
        _ShellCount ("Shell Count", Float) = 32
        _DensityTiling ("Density Tiling", Float) = 32
        _MainTex ("Density Noise (grayscale)", 2D) = "white" {}
    }

    SubShader
    {
        Tags { "Queue"="Transparent" "RenderType"="Transparent" }
        ZWrite Off
        Blend SrcAlpha OneMinusSrcAlpha
        Cull Off

        Pass
        {
            HLSLPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"

            TEXTURE2D(_MainTex); SAMPLER(sampler_MainTex);
            float4 _MainTex_ST;
            float _DebugMode, _ShellIndex, _ShellCount, _DensityTiling;

            struct A { float4 posOS:POSITION; float3 n:NORMAL; float2 uv:TEXCOORD0; };
            struct V { float4 posH:SV_POSITION; float3 n: TEXCOORD0; float2 uv:TEXCOORD1; float sh:TEXCOORD2; };

            V vert(A i){
                V o; o.n = TransformObjectToWorldNormal(i.n);
                o.uv = i.uv * _MainTex_ST.xy + _MainTex_ST.zw;
                o.sh = _ShellIndex / max(_ShellCount,1);
                float4 posWS = float4(TransformObjectToWorld(i.posOS.xyz),1);
                o.posH = TransformWorldToHClip(posWS.xyz);
                return o;
            }

            half4 frag(V i):SV_Target{
                if (_DebugMode < 0.5)         return half4(i.n*0.5+0.5,1);                    // normals
                else if (_DebugMode < 1.5)    return half4(i.uv,0,1);                         // uv
                else if (_DebugMode < 2.5)    return half4(i.sh,1-i.sh,0,1);                  // shell height
                else {
                    float d = SAMPLE_TEXTURE2D(_MainTex, sampler_MainTex, i.uv * _DensityTiling).r;
                    return half4(d,d,d,1);                                                   // density tex
                }
            }
            ENDHLSL
        }
    }
    FallBack Off
}
