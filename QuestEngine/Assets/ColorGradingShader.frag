#version 330 core
#include "ColorManagement.glsl"
in vec2 uv;

uniform sampler2D texture2D;
uniform int texture2DColorSpace;
struct GradingParams {
    vec4 Saturation;
    vec4 Contrast;
    vec4 Lift;
    vec4 Gain;
    vec4 Midtone;
    vec4 Gamma;
    vec4 Offset;
    vec4 Tint;
    float HueShift;
    float MinRange;
    float MaxRange;
    float Padding;
};

uniform GradingParams Shadows;
uniform GradingParams Midtones;
uniform GradingParams Highlights;
uniform GradingParams Global;

uniform float temperature;
uniform float temperatureTint;
uniform float exposure;
out vec4 fragColor;

vec3 RGBtoHSV(vec3 colorIn)
{
	float R = colorIn.r;
    float G = colorIn.g;
    float B = colorIn.b;

    float maxColor = max(max(R, G), B);
    float minColor = min(min(R, G), B);
    float delta = maxColor - minColor;
    
    float V = maxColor;    
    float S = (maxColor == 0.0) ? 0.0 : delta / maxColor;
    
    float H;
    if (delta == 0.0) {
        H = 0.0;
    } else if (maxColor == R) {
        H = 60.0 * mod((G - B) / delta, 6.0);
    } else if (maxColor == G) {
        H = 60.0 * ((B - R) / delta + 2.0);
    } else {
        H = 60.0 * ((R - G) / delta + 4.0);
    }
    
    if (H < 0.0)
        H += 360.0;
    
    return vec3(H / 360.0, S, V);
}

vec3 HSVtoRGB(vec3 colorIN) {
    float H = colorIN.x * 360.0;
    float S = colorIN.y;
    float V = colorIN.z;
    
    float C = V * S;
    float X = C * (1.0 - abs(mod(H / 60.0, 2.0) - 1.0));
    float m = V - C;

    vec3 result;
    
    if (H < 60.0) result = vec3(C, X, 0);
    else if (H < 120.0) result = vec3(X, C, 0);
    else if (H < 180.0) result = vec3(0, C, X);
    else if (H < 240.0) result = vec3(0, X, C);
    else if (H < 300.0) result = vec3(X, 0, C);
    else result = vec3(C, 0, X);
    
    return result + vec3(m);
}

vec3 ApplyContrast(vec3 colorIn, vec3 contrast)
{
    vec3 value = mix(vec3(0.5), colorIn, contrast + 1);
	return value;
}


vec3 ApplySaturation(vec3 colorIN, vec3 saturation)
{
	vec3 rec709LuminanceCoeff = vec3(0.2126, 0.7152, 0.0722);
	float luminance = dot(colorIN,rec709LuminanceCoeff);
	vec3 value = mix(vec3(luminance), colorIN, saturation + 1);
	return value;
}

vec3 ApplyHUE(vec3 colorIn, float hueValue)
{
    vec3 hsv = RGBtoHSV(colorIn);
    hsv.x = mod(hsv.x + hueValue, 1.0f);
    return HSVtoRGB(hsv);
}

vec3 ApplyLift(vec3 colorIn, vec3 liftValue)
{
     return colorIn + liftValue * (1.0 - colorIn);
}

vec3 ApplyMidtone(vec3 colorIn, vec3 midToneValue)
{
    return colorIn + midToneValue * (1.0 - abs(colorIn - 0.5) * 2);
}

float ApplyGamma(float valueIn, float gammaValue)
{
     if (abs(gammaValue) < 1e-5)
        return valueIn;          
    
    if (gammaValue > 0.0f)
        return pow(valueIn, 1.0f / (gammaValue + 1.0f));
    else 
        return pow(valueIn, 1.0f - gammaValue);
}

vec3 ApplyGamma(vec3 colorIn, vec3 gammaValue)
{
    float R = ApplyGamma(colorIn.r, gammaValue.r);
    float G = ApplyGamma(colorIn.g, gammaValue.g);
    float B = ApplyGamma(colorIn.b, gammaValue.b);
    return vec3(R,G,B); 
}


vec3 ApplyGain(vec3 colorIn, vec3 gainValue)
{
     return colorIn + gainValue * colorIn;
}

vec3 ApplyTintColor(vec3 colorIn, vec3 tint)
{
    return colorIn * tint;
}

vec3 ApplyOffset(vec3 colorIn, vec3 offset)
{
    return colorIn + offset;
}

vec3 ApplyTemperatureAndTint(vec3 colorIn, float temperature, float tint)
{
     // Step 1: Convert to LMS space based on colorSpaceIn
    vec3 colorLMS = ConvertToLMS(colorIn, colorSpaceIn);

    // Step 2: Compute the balance coefficients
    vec3 balance = ColorBalanceToLMSCoeffs(temperature, tint);

    // Step 3: Apply the correction
    colorLMS *= balance;

    // Step 4: Convert back to colorSpaceIn
    vec3 correctedColor = ConvertFromLMS(colorLMS, colorSpaceIn);

    return correctedColor;
}

float LuminanceMask(float luma, float minRange, float maxRange) {

    if (minRange >= maxRange)
        return 0.0f;

    if(luma > minRange && luma < maxRange)
        return 1.0f;

        return 0.0f;



    float start = smoothstep(minRange - 0.05, minRange + 0.05, luma);
    float end   = smoothstep(maxRange - 0.05, maxRange + 0.05, luma);

    return start * (1.0 - end);
}

vec3 ApplyGrading(vec3 color, float mask, GradingParams p) {
    vec3 graded = color;

    graded = ApplyHUE(graded, p.HueShift);
    graded = ApplySaturation(graded, p.Saturation.rgb * p.Saturation.w);
    graded = ApplyLift(graded,       p.Lift.rgb * p.Lift.w);
    graded = ApplyMidtone(graded,    p.Midtone.rgb * p.Midtone.w);
    graded = ApplyGain(graded,       p.Gain.rgb * p.Gain.w);
    graded = ApplyContrast(graded,   p.Contrast.rgb * p.Contrast.w);
    graded = ApplyGamma(graded,      p.Gamma.rgb);
    graded = ApplyTintColor(graded,  p.Tint.rgb * p.Tint.w);
    graded = ApplyOffset(graded,     p.Offset.rgb * p.Offset.w);

    return mix(color, graded, mask);
}

vec3 ApplyExposure(vec3 color, float exposure)
{
    return color * pow(2.0, exposure);
}

void main()
{
    
    vec4 color = textureCs(texture2D, uv, texture2DColorSpace);
    color.rgb = ApplyExposure(color.rgb, exposure);
   
    vec3 hsv = RGBtoHSV(color.rgb);
    float value = hsv.z;

    float maskShadow   = LuminanceMask(value, Shadows.MinRange, Shadows.MaxRange);
    float maskMid      = LuminanceMask(value, Midtones.MinRange, Midtones.MaxRange);
    float maskHighlight= LuminanceMask(value, Highlights.MinRange, Highlights.MaxRange);

    color.rgb = ApplyGrading(color.rgb, maskShadow, Shadows);
    color.rgb = ApplyGrading(color.rgb, maskMid, Midtones);
    color.rgb = ApplyGrading(color.rgb, maskHighlight, Highlights);

    color.rgb = ApplyHUE(color.rgb, Global.HueShift);
    color.rgb = ApplyTemperatureAndTint(color.rgb, temperature, temperatureTint);
    color.rgb = ApplyGrading(color.rgb, 1.0, Global);

    color.rgb = ConvertColor(color.rgb, colorSpaceIn, colorSpaceOut);
    fragColor = color;
};