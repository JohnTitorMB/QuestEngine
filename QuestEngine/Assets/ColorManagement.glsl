uniform int colorSpaceIn;
uniform int colorSpaceOut;


// =============== Transfer functions ===============

const float LOG10 = 2.302585092994046;


float sRGBDecode(float encoded)
{
    return (encoded <= 0.04045)
        ? (encoded / 12.92)
        : pow((encoded + 0.055) / 1.055, 2.4);
}

float sRGBEncode(float linear)
{
    return (linear <= 0.0031308)
        ? (12.92 * linear)
        : (1.055 * pow(linear, 1.0 / 2.4) - 0.055);
}

float ACESccDecode(float encoded)
{
    return pow(2.0, (encoded - 0.07290553419583557) * 17.52) - 9.72;
}

float ACESccEncode(float linear)
{
    if (linear <= 0.0)
        return (log(pow(2.0, -16.0) + 9.72) / LOG10 - log(9.72) / LOG10) / (log(2.0) / LOG10 * 17.52) + 0.0729055;
    return (log(linear + 9.72) / LOG10 - log(9.72) / LOG10) / (log(2.0) / LOG10 * 17.52) + 0.0729055;
}

float ACEScctDecode(float encoded)
{
    return (encoded <= 0.155251141552511)
        ? (encoded - 0.07290553419583557) / 10.5402377416545
        : pow(2.0, (encoded - 0.07290553419583557) * 17.52) - 9.72;
}

float ACEScctEncode(float linear)
{
    if (linear <= 0.0078125)
        return 10.5402377416545 * linear + 0.07290553419583557;

    return (log(linear + 9.72) / LOG10 - log(9.72) / LOG10) / (log(2.0) / LOG10 * 17.52) + 0.07290553419583557;
}

// =============== ColorSpace to XYZ functions ===============

// Matrice sRGB -> XYZ
const mat3 sRGBToXYZMatrix = mat3(
    0.4124564, 0.3575761, 0.1804375,
    0.2126729, 0.7151522, 0.0721750,
    0.0193339, 0.1191920, 0.9503041
);

// Matrice ACEScg -> XYZ
const mat3 ACEScgToXYZMatrix = mat3(
    0.9525523959, 0.0000000000, 0.0000936786,
    0.3439664498, 0.7281660966, -0.0721325464,
    0.0000000000, 0.0000000000, 1.0088251844
);

// =============== XYZ to LMS Matrix ===============

const mat3 XYZToLMSMatrix = mat3(
    0.4002, 0.7075, -0.0808,
    -0.2263, 1.1653, 0.0457,
    0.0000, 0.0000, 0.9182
);

// =============== LMS to XYZ Matrix ===============

const mat3 LMSToXYZMatrix = mat3(
    1.8600666, -1.1294800, 0.2198980,
    0.3612225, 0.6388040, -0.0000060,
    0.0000000, 0.0000000, 1.0890640
);

vec3 SrgbToXYZ(vec3 color)
{
    vec3 linear = vec3(
        sRGBDecode(color.r),
        sRGBDecode(color.g),
        sRGBDecode(color.b)
    );
    return sRGBToXYZMatrix * linear;
}

vec3 LinearsrgbToXYZ(vec3 color)
{
    return sRGBToXYZMatrix * color;
}

vec3 ACESccToXYZ(vec3 color)
{
    vec3 linear = vec3(
        ACESccDecode(color.r),
        ACESccDecode(color.g),
        ACESccDecode(color.b)
    );
    return ACEScgToXYZMatrix * linear;
}

vec3 ACEScctToXYZ(vec3 color)
{
    vec3 linear = vec3(
        ACEScctDecode(color.r),
        ACEScctDecode(color.g),
        ACEScctDecode(color.b)
    );
    return ACEScgToXYZMatrix * linear;
}

vec3 ACEScgToXYZ(vec3 color)
{
    return ACEScgToXYZMatrix * color;
}


vec3 ConvertColorToXYZ(vec3 color, int rgbColorSpaceIn)
{
    if (rgbColorSpaceIn == 0)
        return color;

    if (rgbColorSpaceIn == 1) { // sRGB
        return SrgbToXYZ(color);
    }
    else if (rgbColorSpaceIn == 2) { // Linear sRGB
        return LinearsrgbToXYZ(color);
    }
    else if (rgbColorSpaceIn == 3) { // ACEScc
        return ACESccToXYZ(color);
    }
    else if (rgbColorSpaceIn == 4) { // ACEScct
        return ACEScctToXYZ(color);
    }
    else if (rgbColorSpaceIn == 5) { // ACEScg
        return ACEScgToXYZ(color);
    }

    return color;
}

// =============== Matrices XYZ -> RGB ===============

const mat3 XYZToSRGBMatrix = mat3(
    3.2404542, -1.5371385, -0.4985314,
    -0.9692660, 1.8760108, 0.0415560,
    0.0556434, -0.2040259, 1.0572252
);

const mat3 XYZToACEScgMatrix = mat3(
    1.0498110175, 0.0000000000, -0.0000974845,
    -0.4959030231, 1.3733130458, 0.0982400361,
    0.0000000000, 0.0000000000, 0.9912520182
);

// =============== XYZ to ColorSpace functions ===============

vec3 XYZToSrgb(vec3 xyz)
{
    vec3 linear = XYZToSRGBMatrix * xyz;
    return vec3(
        sRGBEncode(linear.r),
        sRGBEncode(linear.g),
        sRGBEncode(linear.b)
    );
}

vec3 XYZToLinearsrgb(vec3 xyz)
{
    return XYZToSRGBMatrix * xyz;
}

vec3 XYZToACEScc(vec3 xyz)
{
    vec3 linear = XYZToACEScgMatrix * xyz;
    return vec3(
        ACESccEncode(linear.r),
        ACESccEncode(linear.g),
        ACESccEncode(linear.b)
    );
}

vec3 XYZToACEScct(vec3 xyz)
{
    vec3 linear = XYZToACEScgMatrix * xyz;
    return vec3(
        ACEScctEncode(linear.r),
        ACEScctEncode(linear.g),
        ACEScctEncode(linear.b)
    );
}

vec3 XYZToACEScg(vec3 xyz)
{
    return XYZToACEScgMatrix * xyz;
}

vec3 ConvertXYZToColor(vec3 xyzColor, int rgbColorSpaceOut)
{
    if (rgbColorSpaceOut == 0)
        return xyzColor;

    if (rgbColorSpaceOut == 1) { // sRGB
        return XYZToSrgb(xyzColor);
    }
    else if (rgbColorSpaceOut == 2) { // Linear sRGB
        return XYZToLinearsrgb(xyzColor);
    }
    else if (rgbColorSpaceOut == 3) { // ACEScc
        return XYZToACEScc(xyzColor);
    }
    else if (rgbColorSpaceOut == 4) { // ACEScct
        return XYZToACEScct(xyzColor);
    }
    else if (rgbColorSpaceOut == 5) { // ACEScg
        return XYZToACEScg(xyzColor);
    }

    // fallback
    return xyzColor;
}

// =============== Constants for White Points ===============

// sRGB / Linear sRGB D65 white point
const vec3 WhitePointD65 = vec3(0.95047, 1.00000, 1.08883);

// ACEScc / ACEScct / ACEScg D60 white point
const vec3 WhitePointD60 = vec3(0.953205, 1.000000, 1.085401);

// =============== Bradford matrices ===============

// Bradford transformation matrix
const mat3 BradfordMatrix = mat3(
    0.8951, 0.2664, -0.1614,
    -0.7502, 1.7135, 0.0367,
    0.0389, -0.0685, 1.0296
);

// Inverse Bradford matrix
const mat3 BradfordMatrixInv = mat3(
    0.9869929, -0.1470543, 0.1599627,
    0.4323053, 0.5183603, 0.0492912,
    -0.0085287, 0.0400428, 0.9684867
);

// =============== Chromatic Adaptation ===============

vec3 ApplyChromaticAdaptation(vec3 xyzColor, vec3 sourceWhiteXYZ, vec3 targetWhiteXYZ)
{
    vec3 sourceLMS = transpose(BradfordMatrix) * sourceWhiteXYZ;
    vec3 targetLMS = transpose(BradfordMatrix) * targetWhiteXYZ;

    mat3 D = mat3(
        targetLMS.x / sourceLMS.x, 0.0, 0.0,
        0.0, targetLMS.y / sourceLMS.y, 0.0,
        0.0, 0.0, targetLMS.z / sourceLMS.z
    );

    mat3 adaptationMatrix = BradfordMatrixInv * D * BradfordMatrix;

    return adaptationMatrix * xyzColor;
}

// =============== ConvertColor function ===============

vec3 ConvertColor(vec3 color, int rgbColorSpaceIn, int rgbColorSpaceOut)
{
     
    // No color space defined or same space, no conversion needed
    if (rgbColorSpaceIn == 0 || rgbColorSpaceOut == 0 || rgbColorSpaceIn == rgbColorSpaceOut)
        return color;

    // Step 1: Convert input color to XYZ space
    vec3 xyz = ConvertColorToXYZ(color, rgbColorSpaceIn);

    // Step 2: Apply chromatic adaptation if needed
    vec3 sourceWhite = WhitePointD65;
    if (rgbColorSpaceIn >= 3 && rgbColorSpaceIn <= 5) { // ACEScc / ACEScct / ACEScg
        sourceWhite = WhitePointD60;
    }

    vec3 targetWhite = WhitePointD65;
    if (rgbColorSpaceOut >= 3 && rgbColorSpaceOut <= 5) { // ACEScc / ACEScct / ACEScg
        targetWhite = WhitePointD60;
    }

    if (sourceWhite != targetWhite)
    {
        xyz = ApplyChromaticAdaptation(xyz, sourceWhite, targetWhite);
    }

    // Step 3: Convert from XYZ space to output color space
    vec3 result = ConvertXYZToColor(xyz, rgbColorSpaceOut);

    return result;
}

vec4 textureCs(sampler2D text, vec2 uv, int textureColorSpace)
{
    vec4 color = texture(text, uv);

    color.rgb = ConvertColor(color.rgb, textureColorSpace, colorSpaceIn);


    return color;
}

vec4 textureCubemapCs(samplerCube cubemap, vec3 pos, int textureColorSpace)
{
    vec4 color = texture(cubemap, pos);
    color.rgb = ConvertColor(color.rgb, textureColorSpace, colorSpaceIn);

    return color;
}

vec3 ConvertToLMS(vec3 color, int colorSpaceIn)
{
    vec3 xyz = ConvertColorToXYZ(color, colorSpaceIn);
    vec3 lms = XYZToLMSMatrix * xyz;
    return lms;
}

vec3 ConvertFromLMS(vec3 lms, int colorSpaceIn)
{
    vec3 xyz = LMSToXYZMatrix * lms;
    vec3 colorOut = ConvertXYZToColor(xyz, colorSpaceIn);
    return colorOut;
}

vec3 KelvinToRGB(float temperatureK) {
    // Limiter la température à l'intervalle utile
    float temp = clamp(temperatureK, 1000.0, 40000.0);
    temp /= 100.0;  // travailler en centaines de Kelvins

    float r, g, b;
    // Calcul du Rouge
    if (temp <= 66.0) {
        r = 1.0;  // Rouge saturé en dessous de ~6600 K
    }
    else {
        // Formule puissance pour R au-delà de 6600 K
        r = 1.292936 * pow(temp - 60.0, -0.1332047592);
        r = clamp(r, 0.0, 1.0);
    }
    // Calcul du Vert
    if (temp <= 66.0) {
        g = 0.3900816 * log(temp) - 0.6318414;
        g = clamp(g, 0.0, 1.0);
    }
    else {
        g = 1.1298909 * pow(temp - 60.0, -0.0755148492);
        g = clamp(g, 0.0, 1.0);
    }
    // Calcul du Bleu
    if (temp <= 19.0) {
        b = 0.0;  // pratiquement pas de bleu en dessous de ~2000 K
    }
    else if (temp >= 66.0) {
        b = 1.0;  // bleu saturé au-dessus de ~6600 K
    }
    else {
        b = 0.5432068 * log(temp - 10.0) - 1.196254;
        b = clamp(b, 0.0, 1.0);
    }
    return vec3(r, g, b);
}

// ===================================
// MATRICES CAT02 pour xy -> LMS
// ===================================

const mat3 CAT02_XYZtoLMS = mat3(
    0.7328, 0.4296, -0.1624,
    -0.7036, 1.6975, 0.0061,
    0.0030, 0.0136, 0.9834
);

const mat3 CAT02_LMSToXYZ = mat3(
    1.096124, -0.278869, 0.182745,
    0.454369, 0.473533, 0.072098,
    -0.009628, -0.005698, 1.015326
);

// ===================================
// Utilities
// ===================================

// Approximation modifiée du standard illuminant
float StandardIlluminantY(float x)
{
    return 2.87 * x - 3.0 * x * x - 0.27509507;
}

// CIE xy chromaticity vers CAT02 LMS
vec3 CIExyToLMS(float x, float y)
{
    float Y = 1.f;
    float X = Y * x / y;
    float Z = Y * (1.f - x - y) / y;

    float L = 0.7328f * X + 0.4296f * Y - 0.1624f * Z;
    float M = -0.7036f * X + 1.6975f * Y + 0.0061f * Z;
    float S = 0.0030f * X + 0.0136f * Y + 0.9834f * Z;

    return vec3(L, M, S);
}

// ===================================
// Color Balance Coeffs
// ===================================

vec3 ColorBalanceToLMSCoeffs(float temperature, float tint)
{

    float t1 = temperature / 65.0;
    float t2 = tint / 65.0;

    float x = 0.31271 - t1 * (t1 < 0.0 ? 0.1 : 0.05);
    float y = StandardIlluminantY(x) + t2 * 0.05;

    // Calcul du nouveau white point LMS
    vec3 w2 = CIExyToLMS(x, y);

    // D65 white point en LMS (CAT02)
    const vec3 D65_LMS = vec3(0.949237, 1.03542, 1.08728);

    // Coeffs = D65 / w2
    return D65_LMS / w2;
}