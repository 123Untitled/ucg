#include "lch.hpp"

/* lch_to_hex
 *
 * convert lch to hex
 *
 * @param lightness
 * @param chroma
 * @param hue
 *
 * @return color::hex
 */

auto color::lch_to_hex( const double lightness,
						const double chroma,
						const double hue) -> color::hex {

	#define LUM(l) (l) < 0 ? 0 : (l) > 100 ? 100 : (l)
	#define HUE(h) (h) < 0 ? 0 : (h) > 360 ? 360 : (h)
	#define CHR(c) (c) < 0 ? 0 : (c) > 100 ? 100 : (c)

								// PI / 180
	const double radian = hue * (0.017453292519943295);

	// convert lab to d50-adapted xyz
	constexpr double k = 24389.0 / 27.0;
	constexpr double e = 216.0 / 24389.0;


	// compute xyz, starting with the luminance-related term
	double y = (lightness + 16) / 116;
	double x =  y + (chroma * std::cos(radian) / 500);
	double z =  y - (chroma * std::sin(radian) / 200);

	// compute x
	x = (std::pow(x, 3) > e ? std::pow(x, 3) : ((116 * x) - 16) / k) * 0.9642956764295677;
	// compute y
	y = (lightness > (k * e) ? std::pow((lightness + 16) / 116, 3) : lightness / k) * 1.00000;
	// compute z
	z = (std::pow(z, 3) > e  ? std::pow(z, 3) : ((116 * z) - 16) / k) * 0.8251046025104602;

	constexpr double d65[3][3] = {
		{  0.9554734527042182,   -0.023098536874261423,  0.0632593086610217   },
		{ -0.028369706963208136,  1.0099954580058226,    0.021041398966943008 },
		{  0.012314001688319899, -0.020507696433477912,  1.3303659366080753   }
	};

	// convert d50-adapted xyz to d65-adapted xyz
	const double d65x = d65[0][0] * x + d65[0][1] * y + d65[0][2] * z;
	const double d65y = d65[1][0] * x + d65[1][1] * y + d65[1][2] * z;
	const double d65z = d65[2][0] * x + d65[2][1] * y + d65[2][2] * z;

	constexpr double mat[3][3] = {
		{  +12831.0 /   +3959.0,     -329.0 /    +214.0,  -1974.0 /   +3959.0 },
		{ -851781.0 / +878810.0, +1648619.0 / +878810.0, +36519.0 / +878810.0 },
		{    +705.0 /  +12673.0,    -2585.0 /  +12673.0,   +705.0 /    +667.0 },
	};

	// convert an array of linear-light srgb values in the range 0.0-1.0
	double r = (mat[0][0] * d65x) + (mat[0][1] * d65y) + (mat[0][2] * d65z);
	double g = (mat[1][0] * d65x) + (mat[1][1] * d65y) + (mat[1][2] * d65z);
	double b = (mat[2][0] * d65x) + (mat[2][1] * d65y) + (mat[2][2] * d65z);

	double abs = 0;

	abs = std::abs(r);
	r = (abs > 0.0031308) ? (r < 0 ? -1 : +1)
		* (1.055 * std::pow(abs, 0.4166666666666667) - 0.055)
		: 12.92 * r;

	abs = std::abs(g);
	g = (abs > 0.0031308) ? (g < 0 ? -1 : +1)
		* (1.055 * std::pow(abs, 0.4166666666666667) - 0.055)
		: 12.92 * g;

	abs = std::abs(b);
	b = (abs > 0.0031308) ? (b < 0 ? -1 : +1)
		* (1.055 * std::pow(abs, 0.4166666666666667) - 0.055)
		: 12.92 * b;

	#define clamp(c) static_cast<unsigned char>(((c) < 0) ? 0 : ((c) > 255) ? 255 : (c))

	//char buffer[256];
	//
	//int size = snprintf(buffer, sizeof(buffer),
	//	"\x1b[48;2;%d;%d;%dm              \x1b[0m\n",
	//	clamp(r * 255), clamp(g * 255), clamp(b * 255));
	//
	//
	//if (size > 0) ::write(1, buffer, static_cast<size_t>(size));


	return color::hex{clamp(r * 255),
					  clamp(g * 255),
					  clamp(b * 255)};

	#undef clamp
}











// Les valeurs de Xn, Yn et Zn sont utilisées comme valeurs de référence pour les couleurs rouge, vert et bleu dans l'espace de couleur XYZ. Elles dépendent du système de couleur et de la source de lumière utilisés. Voici quelques valeurs couramment utilisées :
// coefficients de luminance
// Système de couleur sRGB avec une source de lumière D65 (équivalent à la lumière du jour) :
#define XN 0.95047
#define YN 1.00000
#define ZN 1.08883
/* Système de couleur Adobe RGB avec une source de lumière D65 :
#define XN 0.96422
#define YN 1.00000
#define ZN 0.82521 */
/* Système de couleur NTSC avec une source de lumière C (équivalent à la lumière incandescente) :
#define XN 0.67000
#define YN 1.00000
#define ZN 0.49600 */


// Rn, Gn et Bn sont les valeurs de référence pour les couleurs rouge, vert et bleu dans l'espace de couleur RGB. Elles dépendent également du système de couleur utilisé. Voici quelques valeurs couramment utilisées :
// coefficients de chrominance
/* Système de couleur sRGB : */
#define RN 1.00000
#define GN 1.00000
#define BN 1.00000
// Système de couleur Adobe RGB :
// #define RN 0.96422
// #define GN 1.00000
// #define BN 0.82521
/* Système de couleur NTSC :
#define RN 1.00000
#define GN 1.00000
#define BN 1.00000 */

//auto color::lch_to_hex2(const double lightness,
//						const double chroma,
//						const double hue) -> color::hex {
//
//
//	const double radian = hue * (0.017453292519943295);
//
//	double x, y, z;
//
//	{
//		const double l = lightness;
//		const double a = chroma * std::cos(radian);
//		const double b = chroma * std::sin(radian);
//
//		x = XN * (l + 16) / 116 + a / 500;
//		y = YN * (l + 16) / 116;
//		z = ZN * (l + 16) / 116 - b / 200;
//	}
//
//	const double r = RN * ((+3.2406 * x) - (+1.5372 * y) - (+0.4986 * z));
//	const double g = GN * ((-0.9689 * x) + (+1.8758 * y) + (+0.0415 * z));
//	const double b = BN * ((+0.0557 * x) - (+0.2040 * y) + (+1.0570 * z));
//
//	#define clamp(c) static_cast<unsigned char>((c) < 0 ? 0 : (c) > 255 ? 255 : (c))
//
//	char buffer[256];
//
//	int size = snprintf(buffer, sizeof(buffer),
//		"\x1b[38;2;%d;%d;%dmcolor\x1b[0m\n",
//		clamp(r * 255), clamp(g * 255), clamp(b * 255));
//
//	if (size > 0) ::write(1, buffer, static_cast<size_t>(size));
//
//	return color::hex{clamp(r * 255), clamp(g * 255), clamp(b * 255)};
//}
//
//


/*
Pour convertir une couleur LCH (Luminance, Chrominance, Hue) en RGB (Red, Green, Blue), vous pouvez utiliser une formule mathématique spécifique. Voici les étapes à suivre :

Convertir LCH en LAB : LAB est un espace de couleur qui utilise des valeurs L (luminance), A (chrominance verte-rouge) et B (chrominance bleu-jaune). Pour convertir LCH en LAB, vous pouvez utiliser les formules suivantes :
L' = L
A' = cos(H) * C
B' = sin(H) * C
Où H est l'angle en degrés de la couleur LCH et C est la chrominance.

Convertir LAB en XYZ : XYZ est un espace de couleur qui utilise des valeurs X, Y et Z qui représentent les couleurs primaires rouge, vert et bleu. Pour convertir LAB en XYZ, vous pouvez utiliser les formules suivantes :
X = Xn * (L' + 16) / 116 + A' / 500
Y = Yn * (L' + 16) / 116
Z = Zn * (L' + 16) / 116 - B' / 200
Où Xn, Yn et Zn sont les valeurs de référence pour les couleurs rouge, vert et bleu.

Convertir XYZ en RGB : Pour convertir XYZ en RGB, vous pouvez utiliser les formules suivantes :
R = Rn * (3.2406 * X - 1.5372 * Y - 0.4986 * Z)
G = Gn * (-0.9689 * X + 1.8758 * Y + 0.0415 * Z)
B = Bn * (0.0557 * X - 0.2040 * Y + 1.0570 * Z)
Où Rn, Gn et Bn sont les valeurs de référence pour les couleurs rouge, vert et bleu.
Il est important de noter que les valeurs de LCH et RGB se trouvent dans des espaces de couleur différents et ont donc des échelles de valeurs différentes. Par exemple, la luminance L de LCH varie de 0 à 100, tandis que les valeurs R, G et B de RGB varient généralement de 0 à 255. Vous devrez peut-être adapter ces valeurs en conséquence lors de la conversion.
*/
