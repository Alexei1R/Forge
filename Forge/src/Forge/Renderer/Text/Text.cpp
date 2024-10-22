//
// Created by toor on 2024-10-21.
//


#include "Text.h"


namespace Forge {


Text::Text(std::string filePath)
{
    if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype())
    {
        if (msdfgen::FontHandle* font = loadFont(ft, filePath.c_str()))
        {
            msdfgen::Shape shape;
            if (loadGlyph(shape, font, 'A', msdfgen::FONT_SCALING_EM_NORMALIZED))
            {
                shape.normalize();
                //                      max. angle
                edgeColoringSimple(shape, 3.0);
                //          output width, height
                msdfgen::Bitmap<float, 3> msdf(320, 320);
                //                            scale, translation (in em's)
                msdfgen::SDFTransformation t(
                    msdfgen::Projection(320.0, msdfgen::Vector2(0.125, 0.125)),
                    msdfgen::Range(0.125));
                msdfgen::generateMSDF(msdf, shape, t);
                msdfgen::savePng(msdf, "output.png");
            }
            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
    }
}


}  // namespace Forge
