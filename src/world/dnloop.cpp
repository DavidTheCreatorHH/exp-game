#include "glew.h"
#include "world/dnloop.h"

// Brightness of the world
GLfloat wBrightness = 1.0f;

float lerp(float a, float b, float t){
    return a + (b - a) * t;
}

float change_color(float deltaTime){
    enum TimeState {DAY, SUNSET, NIGHT, SUNRISE, DUSK};
    static TimeState state = DAY;

    static float timer = 0.0f;
    static float t = 0.0f;

    // durations in seconds
    const float DAY_TIME   = 120.0f;
    const float NIGHT_TIME = 120.0f;
    const float TRANS_TIME = 8.f;

    // colors
    const float dayR = 0.59f, dayG = 0.81f, dayB = 0.92f;
    const float nightR = 0.05f, nightG = 0.05f, nightB = 0.10f;
    const float sunsetR = 0.90f, sunsetG = 0.45f, sunsetB = 0.20f;

    float r, g, b;

    timer += deltaTime;

    switch (state)
    {
        case DAY:
            r = dayR; g = dayG; b = dayB;
            if (timer >= DAY_TIME) {
                timer = 0.0f;
                state = SUNSET;
            }
            break;

		case DUSK:
			t = timer / TRANS_TIME;
			r = lerp(sunsetR, nightR, t);
			g = lerp(sunsetG, nightG, t);
			b = lerp(sunsetB, nightB, t);
			if (timer >= TRANS_TIME) {
			    timer = 0.0f;
			    state = NIGHT;
			}
			break;

        case SUNSET:
            t = timer / TRANS_TIME;
            r = lerp(dayR, sunsetR, t);
            g = lerp(dayG, sunsetG, t);
            b = lerp(dayB, sunsetB, t);
            if (timer >= TRANS_TIME) {
                timer = 0.0f;
                state = DUSK;
            }
            break;

        case NIGHT:
            r = nightR; g = nightG; b = nightB;
            if (timer >= NIGHT_TIME) {
                timer = 0.0f;
                state = SUNRISE;
            }
            break;

        case SUNRISE:
            t = timer / TRANS_TIME;
            r = lerp(nightR, dayR, t);
            g = lerp(nightG, dayG, t);
            b = lerp(nightB, dayB, t);
            if (timer >= TRANS_TIME) {
                timer = 0.0f;
                state = DAY;
            }
            break;
    }

    glClearColor(r, g, b, 1.0f);

    float brightness = (r + g + b) / 3.f;
    return brightness;
}
