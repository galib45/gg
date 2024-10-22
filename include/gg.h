#include <raylib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "RobotoMonoRegular.h"

typedef struct {
    FT_Face face;
    Image image;
    Texture2D texture;
} ggFont;

typedef enum {
    GGTEXT_ALIGN_TOP_LEFT,
    GGTEXT_ALIGN_TOP_CENTER,
    GGTEXT_ALIGN_TOP_RIGHT,
    GGTEXT_ALIGN_MIDDLE_LEFT,
    GGTEXT_ALIGN_MIDDLE_CENTER,
    GGTEXT_ALIGN_MIDDLE_RIGHT,
    GGTEXT_ALIGN_BOTTOM_LEFT,
    GGTEXT_ALIGN_BOTTOM_CENTER,
    GGTEXT_ALIGN_BOTTOM_RIGHT
} ggTextAlignment;

static FT_Library ft;
static ggFont ggfont;
static Rectangle windowRect;

bool ggInit() {
    windowRect.width = GetRenderWidth();
    windowRect.height = GetRenderHeight();
    Vector2 position = GetWindowPosition();
    windowRect.x = 0;
    windowRect.y = 0;

    // Initialize FreeType
    if (FT_Init_FreeType(&ft)) {
        printf("Error initializing FreeType library\n");
        return false;
    }

    // Load font
    if (FT_New_Memory_Face(
                ft, ROBOTOMONOREGULAR_DATA, 
                ROBOTOMONOREGULAR_DATA_SIZE, 0, 
                &ggfont.face
    )) {
        printf("Error loading font\n");
        return false;
    }

    // Create image to render text
    ggfont.image = GenImageColor(windowRect.width, windowRect.height, BLANK);
    ggfont.texture = LoadTextureFromImage(ggfont.image);
    return true;
}

bool ggClear() {
    ImageClearBackground(&ggfont.image, BLANK);
    return true;
}

bool ggClean() {
    UnloadTexture(ggfont.texture);
    UnloadImage(ggfont.image);
    // Cleanup
    FT_Done_Face(ggfont.face);
    FT_Done_FreeType(ft);
    return true;
}

Vector2 MeasureTextFT(ggFont *font, char *text, unsigned int fontSize) {
    // Set font size
    Vector2 textSize = {0};
    int width = 0, height = 0;
    FT_Set_Pixel_Sizes(font->face, 0, fontSize);
    for (int i = 0; text[i]; i++) {
        if (FT_Load_Char(font->face, text[i], FT_LOAD_NO_BITMAP)) {
            printf("Error loading character\n");
            continue;
        }

        FT_GlyphSlot slot = font->face->glyph;

        width += slot->advance.x / 64;
        if (height < slot->metrics.height/64) height = slot->metrics.height/64;
    }
    textSize.x = width;
    textSize.y = height; 
    return textSize;
}

void DrawTextFT(
    ggFont *font, char *text, 
    Vector2 position, unsigned int fontSize, 
    Color color, ggTextAlignment alignment
) {
    // Set font size
    FT_Set_Pixel_Sizes(font->face, 0, fontSize);

    int penX = position.x;
    int penY = position.y;
    Vector2 textSize = MeasureTextFT(font, text, fontSize); 

    int adjustX = 0, adjustY = 0;
    switch (alignment) {
        case GGTEXT_ALIGN_TOP_LEFT: break;
        case GGTEXT_ALIGN_TOP_CENTER: adjustX = textSize.x / 2; break;
        case GGTEXT_ALIGN_TOP_RIGHT: adjustX = textSize.x; break;
        case GGTEXT_ALIGN_MIDDLE_LEFT: adjustY = textSize.y / 2; break;
        case GGTEXT_ALIGN_MIDDLE_CENTER: adjustX = textSize.x / 2; adjustY = textSize.y / 2; break;
        case GGTEXT_ALIGN_MIDDLE_RIGHT: adjustX = textSize.x; adjustY = textSize.y / 2; break;
        case GGTEXT_ALIGN_BOTTOM_LEFT: adjustY = textSize.y; break;
        case GGTEXT_ALIGN_BOTTOM_CENTER: adjustX = textSize.x / 2; adjustY = textSize.y; break;
        case GGTEXT_ALIGN_BOTTOM_RIGHT: adjustX = textSize.x; adjustY = textSize.y; break;
    }
    // Render text to image
    for (int i = 0; text[i]; i++) {
        if (FT_Load_Char(font->face, text[i], FT_LOAD_RENDER)) {
            printf("Error loading character\n");
            continue;
        }

        FT_GlyphSlot slot = font->face->glyph;
        Color pixelColor;
        unsigned char bitmapAlpha;

        for (int y = 0; y < slot->bitmap.rows; y++) {
            for (int x = 0; x < slot->bitmap.width; x++) {
                bitmapAlpha = slot->bitmap.buffer[y * slot->bitmap.width + x];
                pixelColor = color;
                pixelColor.a = (pixelColor.a * bitmapAlpha) / 255;
                if (pixelColor.a != 0)
                    ImageDrawPixel(&font->image, penX + x + slot->bitmap_left - adjustX, penY + y - slot->bitmap_top - adjustY, pixelColor);
            }
        }

        penX += slot->advance.x >> 6;
    }

    UpdateTexture(font->texture, font->image.data);
    DrawTexture(font->texture, 0, 0, WHITE);
}

void ggDrawText(char *text, Vector2 position) { 
    DrawTextFT(&ggfont, text, position, 16, BLACK, GGTEXT_ALIGN_TOP_LEFT); 
}

void ggDrawTextEx(char *text, Vector2 position, unsigned int fontSize, Color color) { 
    DrawTextFT(&ggfont, text, position, fontSize, color, GGTEXT_ALIGN_TOP_LEFT); 
}

void ggDrawTextExAligned
(char *text, Vector2 position, unsigned int fontSize, Color color, ggTextAlignment alignment) { 
    DrawTextFT(&ggfont, text, position, fontSize, color, alignment); 
}

bool ggDrawTextPro(
    ggFont *font, char *text, Vector2 position,
    unsigned int fontSize, Color color, ggTextAlignment alignment
) { 
    bool result = false;
    DrawTextFT(font, text, position, fontSize, color, alignment);
    Vector2 textSize = MeasureTextFT(font, text, fontSize);
    int adjustX = 0, adjustY = 0;
    switch (alignment) {
        case GGTEXT_ALIGN_BOTTOM_LEFT: break;
        case GGTEXT_ALIGN_BOTTOM_CENTER: adjustX = textSize.x / 2; break;
        case GGTEXT_ALIGN_BOTTOM_RIGHT: adjustX = textSize.x; break;
        case GGTEXT_ALIGN_MIDDLE_LEFT: adjustY = textSize.y*3/2; break;
        case GGTEXT_ALIGN_MIDDLE_CENTER: adjustX = textSize.x / 2; adjustY = textSize.y*3/2; break;
        case GGTEXT_ALIGN_MIDDLE_RIGHT: adjustX = textSize.x; adjustY = textSize.y*3/2; break;
        case GGTEXT_ALIGN_TOP_LEFT: adjustY = textSize.y; break;
        case GGTEXT_ALIGN_TOP_CENTER: adjustX = textSize.x / 2; adjustY = textSize.y; break;
        case GGTEXT_ALIGN_TOP_RIGHT: adjustX = textSize.x; adjustY = textSize.y; break;
    }
    Rectangle bounds = (Rectangle) { position.x - adjustX, position.y - adjustY, textSize.x, textSize.y };
    // DrawRectangleLinesEx(bounds, 2.0, RED);
    // TraceLog(LOG_INFO, "%f, %f, %f, %f", bounds.x, bounds.y, bounds.width, bounds.height);

    if(CheckCollisionPointRec(GetMousePosition(), bounds)) {
        // TraceLog(LOG_INFO, "inside");
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
            result = true;
    }
    return result;
}

