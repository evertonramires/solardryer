
// INPUTS: sensorStatus[0~6], sensorTemp[0~6], sensorHum[0~6]
// OUTPUTS: fanSpeed[0~6] = 0~255, valveStatus[0~6] = 0~1, peltiersPower = 0~255

/*
Energy
E=m⋅cp⋅ΔT

Absolute humidity
HA =( HR x Pw) / (461,5 J⋅kg⁻¹⋅K⁻¹) × T [g/kgairsec]
461,5 J⋅kg⁻¹⋅K⁻¹ est la constante des gaz spécifique pour la vapeur d’eau.

Masse volumique de l’air humide
ρ = (Psec/RaT)+( Pw/RwT)
Rw : la constante de la vapeur d'eau, égale à 461,495 J⋅kg-1⋅K-1

La masse volumique de l’air sec
ρ0 = Patm / (R⋅T)
R – la constante de l'air sec, égale à 287,058 J⋅kg-1⋅K-1

La pression partielle de la vapeur d’eau
Pw=HR×Psat [hPa]

La pression de vapeur saturante
Psat=0.61078·e^((17.27·T)/(T+237.3)) [kPa]

Des valeurs exemple de pression de vapeur saturante
- T = 20°C : Pws ≈ 23,4 hPa
- T = 30°C : Pws ≈ 43,2 hPa
- T = 40°C : Pws ≈ 76,1 hPa

CP de l’air humide
cp=(1-W)*Cpas + W·Cpvap

Rapport de mélange massique W
W=0.622×(Pw)/(Patm−Pw) [kgeau/kgair]

Cp de la vapeur d’eau
Cp (J/kg·K) = 1843,5 + 0,695 T (K)

Enthalpie de l’air humide
Enthalpie de l’air humide (H) = Enthalpie de l’air sec (Has) + Chaleur latente de vapeur
d’eau (Lv)

Enthalpie de l’air sec (Has)
Has = Cpas × T

Chaleur latente de vapeur d’eau (Lv)
Lv = W. (2 501 + 1,83 . T[°C] ) [kJ/kgairsec]

Condensation de la vapeur d’eau par le froid
m_eau = (H - Hs) / (Lv(0°C) + Cpeau × (T - 0°C))

Calculs d'évaporation eau au séchage air sec équations
Équation de Willis Carrier (1911) :
pv(θh) = pvs(θh) - [(p - pvs(θh)) * (θ - θh) / (1532,4 - 1,3 * θh)]


Details:
https://ideavox.org/system/files/2024-12/A037-Rapport-Sechoir-solaire-hybride.pdf
*/

void secretSauce()
{
  peltiersPower = 255;

  fanSpeed[0] = 0;
  fanSpeed[1] = 25;
  fanSpeed[2] = 55;
  fanSpeed[3] = 100;
  fanSpeed[4] = 155;
  fanSpeed[5] = 200;
  fanSpeed[6] = 255;

  valveStatus[6] = 1;
}