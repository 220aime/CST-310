Shader "AdvancedFur/FurShaderVariants"
{
    Properties
    {
        _MainTex       ("Fur Pattern (grayscale noise)", 2D) = "white" {}
        _ColorMap      ("Tiger Color Map (stripes)", 2D) = "white" {}
        _FurColor      ("Global Tint", Color) = (1,1,1,1)

        _FurLength     ("Fur Length", Range(0,1)) = 0.25
        _FurDensity    ("Fur Density (tiling)", Range(1,128)) = 32
        _Cutoff        ("Alpha Cutoff Start", Range(0,1)) = 0.45
        _CutoffEnd     ("Alpha Cutoff End", Range(0,1)) = 0.85

        _ShellIndex    ("Shell Index", Float) = 0
        _ShellCount    ("Shell Count", Float) = 32
        _WindDirection ("Wind Direction", Vector) = (0,0,0,0)
        _Gravity       ("Gravity Strength", Range(0,1)) = 0.25
        _Ambient       ("Ambient Boost", Range(0,2)) = 0.30

        // Stripe controls (how dark stripes affect fur)
        _StripeToLength  ("Dark Stripe -> Length Mult", Range(0,1)) = 0.85
        _StripeToDensity ("Dark Stripe -> Density Mult", Range(1,2)) = 1.25
        _StripeContrast  ("Stripe Contrast", Range(0,2)) = 1.0
    }

    SubShader
    {
        Tags { "RenderType"="Transparent" "Queue"="Transparent" "IgnoreProjector"="True" }
        Blend SrcAlpha OneMinusSrcAlpha
        Cull Off
        ZWrite Off

        Pass
        {
            Name "SHELLS"
            HLSLPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.0
            #pragma multi_compile_instancing

            // URP includes
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"
            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"

            // Textures/uniforms
            TEXTURE2D(_MainTex);  SAMPLER(sampler_MainTex);   float4 _MainTex_ST;
            TEXTURE2D(_ColorMap); SAMPLER(sampler_ColorMap);  float4 _ColorMap_ST;

            float4 _FurColor;
            float  _FurLength, _FurDensity, _Cutoff, _CutoffEnd;
            float  _ShellIndex, _ShellCount, _Gravity, _Ambient;
            float  _StripeToLength, _StripeToDensity, _StripeContrast;
            float4 _WindDirection;

            struct Attributes {
                float4 positionOS : POSITION;
                float3 normalOS   : NORMAL;
                float2 uv         : TEXCOORD0;
                UNITY_VERTEX_INPUT_INSTANCE_ID
            };

            struct Varyings {
                float4 positionHCS : SV_POSITION;
                float3 normalWS    : TEXCOORD0;
                float2 uvMain      : TEXCOORD1;   // for _MainTex
                float2 uvColor     : TEXCOORD2;   // for _ColorMap
                float  shellH      : TEXCOORD3;
                float  stripeDark  : TEXCOORD4;   // 0=orange, 1=black stripe
                UNITY_VERTEX_INPUT_INSTANCE_ID
            };

            // luminance helper
            inline float Luma(float3 c) { return dot(c, float3(0.299, 0.587, 0.114)); }

            Varyings vert (Attributes IN)
            {
                UNITY_SETUP_INSTANCE_ID(IN);
                Varyings OUT;

                OUT.uvMain  = IN.uv * _MainTex_ST.xy  + _MainTex_ST.zw;
                OUT.uvColor = IN.uv * _ColorMap_ST.xy + _ColorMap_ST.zw;

                float shellH = _ShellIndex / max(_ShellCount, 1.0);
                OUT.shellH = shellH;

                // world data
                float3 posWS = TransformObjectToWorld(IN.positionOS.xyz);
                float3 nWS   = TransformObjectToWorldNormal(IN.normalOS);

                // sample tiger color in VERTEX to modulate length (LOD 0 safe here)
                float3 tigerRGB = SAMPLE_TEXTURE2D_LOD(_ColorMap, sampler_ColorMap, OUT.uvColor, 0).rgb;
                float   tigerLum = Luma(tigerRGB);
                float   dark = pow(1.0 - tigerLum, _StripeContrast);  // black areas -> 1
                OUT.stripeDark = saturate(dark);

                // shorter hair in dark stripes for sharper pattern
                float lenMult = lerp(1.0, _StripeToLength, OUT.stripeDark);

                // offset along normal + simple bend
                float3 gravity = float3(0, -_Gravity * shellH, 0);
                float3 wind    = _WindDirection.xyz * shellH;
                posWS += nWS * (_FurLength * lenMult * shellH) + gravity + wind;

                OUT.normalWS = nWS;

                // world->clip + tiny per-shell z-bias to reduce inter-layer fighting
                OUT.positionHCS = TransformWorldToHClip(posWS);
                OUT.positionHCS.z += _ShellIndex * 1e-4;

                return OUT;
            }

            half4 frag (Varyings IN) : SV_Target
            {
                // density scaled up in dark stripes (thicker-looking black bands)
                float densityScale = lerp(1.0, _StripeToDensity, IN.stripeDark);
                float2 tiledUV = IN.uvMain * (_FurDensity * densityScale);
                float furPattern = SAMPLE_TEXTURE2D(_MainTex, sampler_MainTex, tiledUV).r;

                // per-shell cutoff ramp
                float cutoff = lerp(_Cutoff, _CutoffEnd, IN.shellH);
                if (furPattern < cutoff) discard;

                // lighting: main light + constant ambient
                Light mainLight = GetMainLight();
                float3 L = normalize(-mainLight.direction);
                float  NdotL = saturate(dot(IN.normalWS, L));

                // tiger base color from color map (sRGB should be ON for this texture)
                float3 tigerRGB = SAMPLE_TEXTURE2D(_ColorMap, sampler_ColorMap, IN.uvColor).rgb;

                float3 ambient = _Ambient.xxx;
                float3 lit = tigerRGB * _FurColor.rgb * (ambient + NdotL * mainLight.color.rgb);

                // alpha thins towards tips
                float alpha = (1.0 - IN.shellH) * smoothstep(cutoff - 0.1, cutoff + 0.1, furPattern);
                return half4(lit, saturate(alpha));
            }
            ENDHLSL
        }
    }

    FallBack Off
}
