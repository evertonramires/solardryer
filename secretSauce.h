
#include <math.h>
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
  // control all other fans using potentiomenter
  // fanSpeed[0] = globalFanSpeed;
  fanSpeed[1] = globalFanSpeed;
  fanSpeed[2] = globalFanSpeed;
  // fanSpeed[3] = globalFanSpeed;
  fanSpeed[4] = globalFanSpeed;
  fanSpeed[5] = globalFanSpeed;
  // fanSpeed[6] = globalFanSpeed;

  // Magnus coeffficients
  // a = 17.625 °C and b = 243.04 °C
  // Ts = (b × α(T,RH)) / (a - α(T,HR))
  // α(T,HR) = ln(HR/100) + aT/(b+T)

  // dewPoint = 243.04 * (log(sensorHum[0] / 100) + (17.625 * sensorTemp[0]) / (243.04 + sensorTemp[0])) / (17.625 - log(sensorHum[0] / 100) - (17.625 * sensorTemp[0]) / (243.04 + sensorTemp[0]));
  // Serial.println(dewPoint);

  const float a = 17.27;
  const float b = 237.7;

  float T = sensorTemp[0]; // Température en °C
  float RH = sensorHum[0]; // Humidité relative en %

  float alpha = (a * T) / (b + T) + log(RH / 100.0);
  float Td = (b * alpha) / (a - alpha);

  dewPoint = Td;
  Serial.println(dewPoint);

  // ################## DEBUG ##################
  //  ########### THIS IS A VERY SIMPLE BANG-BANG LOGIC TO DEBUG HARDWARE ###########

  // 0 BEFORE PELTIERS
  // 1 RIGHT AFTER PELTIERS
  // 3 SOLAR BODY
  // 6 DRY BOX (OUT)

  // if hotter than expected, turn on cold air intake
  if (sensorTemp[6] > DEBUG_TARGET_TEMP + DEBUG_TARGET_OFFSET)
  {
    fanSpeed[0] = 255;  // cold air intake at full speed
    fanSpeed[3] = 0;    // hot air intake off
    fanSpeed[6] = 255;  // outlet fan at full speed
    valveStatus[3] = 0; // close sun body inlet valve
  }
  // if colder than expected, turn on hot air intake
  else if (sensorTemp[6] < DEBUG_TARGET_TEMP - DEBUG_TARGET_OFFSET)
  {
    valveStatus[3] = 1; // open sun body inlet valve
    fanSpeed[0] = 0;    // cold air intake off
    fanSpeed[3] = 255;  // hot air intake af full speed
    fanSpeed[6] = 255;  // outlet fan at full speed
  }
  // if within range, turn off all fans
  else
  {
    fanSpeed[0] = 0; // cold air intake off
    fanSpeed[3] = 0; // hot air intake off
    fanSpeed[6] = 0; // outlet fan off
  }

  // if more humid than expected and hotter than dewPoint, turn on peltiers
  if ((sensorHum[1] > DEBUG_TARGET_HUM + DEBUG_TARGET_OFFSET) && (sensorTemp[1] > dewPoint) && (dewPoint > 0))
  {
    peltiersPower = 255;
  }
  // if less humid than expected, turn off peltiers
  else if ((sensorHum[1] < DEBUG_TARGET_HUM - DEBUG_TARGET_OFFSET))
  {
    peltiersPower = 0;
  }
  else
  {
    peltiersPower = 0;
  }
}