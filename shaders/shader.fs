#version 330 core

in vec2 TexCoord;

out vec4 FragColor;
layout(location = 0) out vec4 color_out;

uniform sampler2D textureUnit;
uniform vec2 mousePosU;
uniform int widthU;
uniform int heightU;
uniform int drawCircleSizeU;
uniform int mouseLeftButtonU;
uniform int sandModeU;
uniform int waterModeU;
uniform int wallModeU;
uniform int eraserModeU;

vec4 getTexColor(vec2 off);
vec4 calcColor();
bool equalFloat(float a, float b);
bool eqCol(vec4 color1, vec4 color2);
bool neqCol(vec4 color1, vec4 color2);

float width = float(widthU);
float height = float(heightU);

vec4 black = vec4(0.0, 0.0, 0.0, 0.0);
vec4 sand_color = vec4(0.70, 0.40, 0.0, 1.0);
vec4 water_color = vec4(0.33, 0.69, 0.93, 1.0);
vec4 wall_color = vec4(0.50, 0.50, 0.50, 1.0);
vec2 unitPixel = vec2(1.0, 1.0) / vec2(widthU, heightU);

vec2 up = vec2(0.0, 1.0);
vec2 down = vec2(0.0, -1.0);
vec2 left = vec2(-1.0, 0.0);
vec2 right = vec2(1.0, 0.0);
vec2 here = vec2(0.0, 0.0);

vec4 upColor = getTexColor(up);
vec4 downColor = getTexColor(down);
vec4 hereColor = getTexColor(here);
vec4 leftColor = getTexColor(left);
vec4 leftleftColor = getTexColor(left + left);
vec4 leftleftleftColor = getTexColor(left + left + left);
vec4 rightColor = getTexColor(right);
vec4 rightrightColor = getTexColor(right + right);
vec4 downleftColor = getTexColor(down + left);
vec4 downleftleftColor = getTexColor(down + left + left);
vec4 downrightColor = getTexColor(down + right);
vec4 downrightrightColor = getTexColor(down + right + right);
vec4 upleftColor = getTexColor(up + left);
vec4 upleftleftColor = getTexColor(up + left + left);
vec4 uprightColor = getTexColor(up + right);

vec2 pos = gl_FragCoord.xy / vec2(width, height);

// circle arround the mouse
float isIn = step(sqrt(pow((mousePosU.x - pos.x) * width, 2) + pow((mousePosU.y - pos.y) * height, 2)), drawCircleSizeU);

// if the mouse is clicked, draws particles arround the circle
vec4 clicked_circle = isIn * mouseLeftButtonU * (sandModeU * sand_color + waterModeU * water_color + wallModeU * wall_color + eraserModeU * black);

void main() {
    vec4 res = calcColor();
    FragColor = res;
    color_out = res;
}

bool equalFloat(float a, float b){
    return abs(a - b) < 0.01;
}

bool eqCol(vec4 color1, vec4 color2){
    return equalFloat(color1.x, color2.x) && equalFloat(color1.y, color2.y) && equalFloat(color1.z, color2.z);
}

bool neqCol(vec4 color1, vec4 color2){
    return !eqCol(color1, color2);
}

vec4 calcColor(){

    if(eraserModeU * isIn * mouseLeftButtonU == 1) return black;
    if((hereColor == black) && (upColor == black) && (downColor == black) && (rightColor == black) && (leftColor == black)) return clicked_circle;

    // wall rules
    if(eqCol(wall_color, hereColor)) return hereColor;

    // SAND RULES

    // dont falling off from screen
    if(pos.y <= unitPixel.y && eqCol(hereColor, sand_color)) return hereColor;
    if(pos.x <= unitPixel.x && eqCol(upColor, sand_color)) return upColor;
    if(pos.x <= unitPixel.x && eqCol(uprightColor, sand_color)) return uprightColor;
    if(pos.x >= (unitPixel.x * width) && eqCol(upColor, sand_color)) return upColor;
    if(pos.x >= (unitPixel.x * (width - 1)) && eqCol(upleftColor, sand_color)) return upleftColor;

    // falling
    if(eqCol(hereColor, black) && eqCol(upColor, sand_color)) return upColor;
    if(eqCol(hereColor, sand_color) && eqCol(downColor, black)) return black;

    // sand goes through water
    if(eqCol(hereColor, sand_color) && eqCol(downColor, water_color)) return black;
    if(eqCol(hereColor, water_color) && eqCol(upColor, sand_color)) return upColor;

    //stacking up (on sand or water or on wall) or sliding if posible
    if(eqCol(hereColor, sand_color) && (eqCol(downColor, sand_color) || eqCol(downColor, water_color) || eqCol(downColor, wall_color)))
        if(eqCol(downleftColor, sand_color) && eqCol(rightColor, black) && eqCol(downrightColor, black) && eqCol(rightrightColor, black) && eqCol(upColor,black) && eqCol(uprightColor,black))  //fall to the right
            return black;
        else if(eqCol(downrightColor, sand_color) && eqCol(leftColor, black) && eqCol(downleftColor, black) && eqCol(leftleftColor, black) && eqCol(upColor, black) && eqCol(upleftColor, black))  //fall to the left
            return black;
        else if((leftleftColor == black) && (downleftColor == black) && (leftColor == black) && (upleftColor == black) && (upColor == black) && (uprightColor == black) && (rightColor == black) && (rightrightColor == black) && (downrightColor == black)) //fal to one of both sides
            return black;
        else
            return hereColor;

    //sliding coloring rules
    if(eqCol(hereColor, black) && eqCol(leftColor, sand_color) && eqCol(downleftColor, sand_color) && eqCol(downleftleftColor, sand_color) && eqCol(rightColor, black) && eqCol(upColor, black) && eqCol(upleftColor, black) && eqCol(downColor, black))
        return leftColor;
    if(eqCol(hereColor, black) && eqCol(rightColor, sand_color) && eqCol(downrightColor, sand_color) && eqCol(downrightrightColor, sand_color) && eqCol(leftColor, black) && eqCol(upColor, black) && eqCol(uprightColor, black) && eqCol(downColor, black))
        return rightColor;
    if(eqCol(hereColor, black) && eqCol(downColor, black) && eqCol(rightColor, black) && eqCol(upColor, black) && eqCol(leftColor, sand_color) && eqCol(downleftColor, sand_color) && eqCol(upleftColor, black) && eqCol(upleftleftColor, black) && eqCol(leftleftColor, black) && eqCol(downleftleftColor, black) && eqCol(leftleftleftColor, black))
        return leftColor;

    // END SAND RULES

    // WATER RULES

    // don't falling off from screen
    if(pos.y <= unitPixel.y && eqCol(hereColor, water_color)) return hereColor;
    if(pos.x <= unitPixel.x && eqCol(upColor, water_color)) return upColor;
    if(pos.x <= unitPixel.x && eqCol(uprightColor, water_color)) return uprightColor;
    if(pos.x >= (unitPixel.x * width) && eqCol(upColor, water_color)) return upColor;
    if(pos.x >= (unitPixel.x * (width - 1)) && eqCol(upleftColor, water_color)) return upleftColor;

    // falling
    if(eqCol(hereColor, black) && eqCol(upColor, water_color)) return upColor;

    //on a wall or on sand the water will go horizontaly
    if((eqCol(downColor, wall_color) || pos.y <= unitPixel.y) && eqCol(hereColor, black) && eqCol(leftColor, water_color)) return leftColor;
    if(eqCol(hereColor, black) && eqCol(leftColor, water_color) && (eqCol(downleftColor, wall_color) || eqCol(downleftColor, sand_color))) return water_color;
    if((eqCol(downColor, wall_color) || pos.y <= unitPixel.y) && eqCol(hereColor, black) && eqCol(rightColor, water_color)) return rightColor;
    if(eqCol(hereColor, black) && eqCol(rightColor, water_color) && (eqCol(downrightColor, wall_color) || eqCol(downrightColor, sand_color))) return water_color;

    if(eqCol(hereColor, water_color) && eqCol(downColor, black)) return black;
    if(eqCol(hereColor, water_color) && (eqCol(leftColor, sand_color) || eqCol(leftColor, wall_color))) return water_color;
    if(eqCol(hereColor, water_color) && (eqCol(rightColor, sand_color) || eqCol(rightColor, wall_color))) return water_color;

    // END WATER RULES

    return clicked_circle;
}

vec4 getTexColor(vec2 off){
    if(TexCoord.y + unitPixel.y > height + 1) return black;
    if(TexCoord.y + unitPixel.y < -1) return black;

    return texture(textureUnit, vec2(TexCoord.x + unitPixel.x * off.x, TexCoord.y + unitPixel.y * off.y));
}