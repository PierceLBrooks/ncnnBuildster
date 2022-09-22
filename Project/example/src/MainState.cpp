#include <MainState.hpp>
#include <iostream>

#define PI 4.0f*atanf(1.0f)

sfml::MainState* sfml::MainState::instance = nullptr;

sfml::MainState::MainState() :
	State()
{
	instance = this;
}

bool sfml::MainState::checkDraw() const
{
    if ((drawing) && (cursor.x >= 0.0f) && (cursor.y >= 0.0f))
    {
        return true;
    }
    return false;
}

void sfml::MainState::enter()
{
    dimension = 1;
    drawableStack = new DrawableStack();
    drawMode = 0;
    scale = 3;
    deltaTime = 0;
    gray = 0;
    mouse.x = 0;
    mouse.y = 0;
    mousePrev.x = 0;
    mousePrev.y = 0;
    mouseReal.x = 0;
    mouseReal.y = 0;
    brushSize = 0;
    brushSizeMax = 25;
    drawing = false;
    brushMode = false;
    inferMode = false;
    brushColor = sf::Color::Black;
    font = new sf::Font();
    font->loadFromFile("rmono.ttf");
    pictureImg = new sf::Image();
    dimension = static_cast<unsigned int>(std::min((window.x/scale)*.75f,window.y/scale));
    pictureImg->create(dimension,dimension,sf::Color::White);
    pictureTex = new sf::Texture();
    pictureTex->loadFromImage(*pictureImg);
    pictureSpr = new sf::Sprite(*pictureTex);
    pictureSpr->setScale(scale,scale);
    brushImg = new sf::Image();
    brushImg->create(brushSizeMax*2, brushSizeMax*2, sf::Color::Transparent);
    brushImg->setPixel(brushSizeMax,brushSizeMax,sf::Color(128,128,128,128));
    brushTex = new sf::Texture();
    brushTex->loadFromImage(*brushImg);
    brushSpr = new sf::Sprite(*brushTex);
    brushSpr->setScale(scale,scale);
    brushSpr->setOrigin(brushSizeMax, brushSizeMax);
    background = new sf::RectangleShape();
    background->setSize(sf::Vector2f(dimension,dimension)*static_cast<float>(scale));
    background->setFillColor(sf::Color(192,192,192));
    foreground = new sf::RectangleShape();
    foreground->setSize(sf::Vector2f(window.x*.25f,window.y));
    foreground->setPosition(window.x-(window.x*.25f),0);
    foreground->setFillColor(sf::Color(128,128,128));
    colorSelect = new sf::RectangleShape();
    colorSelect->setSize(sf::Vector2f(256,32));
    colorSelect->setFillColor(sf::Color::Black);
    colorSelect->setPosition(window.x-(window.x*.25f)+32,window.y/16);
    colorSelect->setOutlineColor(sf::Color::Black);
    colorSelect->setOutlineThickness(2);
    paletteColorOutline = new sf::RectangleShape();
    paletteColorOutline->setSize(sf::Vector2f(256,32));
    paletteColorOutline->setFillColor(sf::Color::Black);
    paletteColorOutline->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*2);
    paletteColorOutline->setOutlineColor(sf::Color::Black);
    paletteColorOutline->setOutlineThickness(2);
    paletteGrayOutline = new sf::RectangleShape();
    paletteGrayOutline->setSize(sf::Vector2f(256,32));
    paletteGrayOutline->setFillColor(sf::Color::Black);
    paletteGrayOutline->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*3);
    paletteGrayOutline->setOutlineColor(sf::Color::Black);
    paletteGrayOutline->setOutlineThickness(2);
    paletteColorTex = new sf::RenderTexture();
    paletteColorTex->create(256,32);
    paletteColorTex->setActive();
    paletteGrayTex = new sf::RenderTexture();
    paletteGrayTex->create(256,32);
    paletteGrayTex->setActive();
    for (int i = 0; i < 255; i++)
    {
        colorUnit = new sf::RectangleShape();
        colorUnit->setFillColor(colorConvert(sf::Vector3f(((float)i/255.0f)*359.0f,1,1)));
        colorUnit->setSize(sf::Vector2f(1,32));
        colorUnit->setPosition(i,0);
        paletteColorTex->draw(*colorUnit);
        colorUnit = new sf::RectangleShape();
        colorUnit->setFillColor(sf::Color(i,i,i));
        colorUnit->setSize(sf::Vector2f(1,32));
        colorUnit->setPosition(i,0);
        paletteGrayTex->draw(*colorUnit);
    }
    paletteColorTex->setActive(false);
    paletteColorSpr = new sf::Sprite(paletteColorTex->getTexture());
    paletteColorSpr->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*2);
    paletteGrayTex->setActive(false);
    paletteGraySpr = new sf::Sprite(paletteGrayTex->getTexture());
    paletteGraySpr->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*3);
    brushSizeSliderBack = new sf::RectangleShape();
    brushSizeSliderBack->setSize(sf::Vector2f(256,32));
    brushSizeSliderBack->setFillColor(sf::Color::White);
    brushSizeSliderBack->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*4);
    brushSizeSliderBack->setOutlineColor(sf::Color::Black);
    brushSizeSliderBack->setOutlineThickness(2);
    brushSizeSlider = new sf::RectangleShape();
    brushSizeSlider->setSize(sf::Vector2f(8,32));
    brushSizeSlider->setFillColor(sf::Color::Black);
    brushSizeSlider->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*4);
    infer = new sf::RectangleShape();
    infer->setSize(sf::Vector2f(256,32));
    infer->setFillColor(sf::Color::White);
    infer->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*5);
    infer->setOutlineColor(sf::Color::Black);
    infer->setOutlineThickness(2);
    erase = new sf::RectangleShape();
    erase->setSize(sf::Vector2f(256,32));
    erase->setFillColor(sf::Color::White);
    erase->setPosition(window.x-(window.x*.25f)+32,(window.y/16)*6);
    erase->setOutlineColor(sf::Color::Black);
    erase->setOutlineThickness(2);
    colorSelectTxt = new sf::Text(sf::String("Brush Color"), *font, 20);
    colorSelectTxt->setPosition(colorSelect->getPosition().x+128,colorSelect->getPosition().y+4);
    colorSelectTxt->setFillColor(sf::Color::White);
    colorSelectTxt->setOrigin(colorSelectTxt->getGlobalBounds().width/2,0);
    colorSelectTxt->setStyle(sf::Text::Bold);
    brushSizeSliderTxt = new sf::Text(sf::String("Brush Size"), *font, 20);
    brushSizeSliderTxt->setPosition(brushSizeSliderBack->getPosition().x+128,brushSizeSliderBack->getPosition().y+4);
    brushSizeSliderTxt->setFillColor(sf::Color(128,128,128));
    brushSizeSliderTxt->setOrigin(brushSizeSliderTxt->getGlobalBounds().width/2,0);
    brushSizeSliderTxt->setStyle(sf::Text::Bold);
    inferTxt = new sf::Text(sf::String("Infer"), *font, 20);
    inferTxt->setPosition(infer->getPosition().x+128,infer->getPosition().y+4);
    inferTxt->setFillColor(sf::Color(128,128,128));
    inferTxt->setOrigin(inferTxt->getGlobalBounds().width/2,0);
    inferTxt->setStyle(sf::Text::Bold);
    eraseTxt = new sf::Text(sf::String("Erase"), *font, 20);
    eraseTxt->setPosition(erase->getPosition().x+128,erase->getPosition().y+4);
    eraseTxt->setFillColor(sf::Color(128,128,128));
    eraseTxt->setOrigin(eraseTxt->getGlobalBounds().width/2,0);
    eraseTxt->setStyle(sf::Text::Bold);
    drawableStack->addDrawable(brushSpr, 3);
    drawableStack->addDrawable(background, 1);
    drawableStack->addDrawable(pictureSpr, 2);
    drawableStack->addDrawable(foreground, 4);
    drawableStack->addDrawable(colorSelect, 5);
    drawableStack->addDrawable(paletteColorSpr, 6);
    drawableStack->addDrawable(paletteGraySpr, 6);
    drawableStack->addDrawable(brushSizeSlider, 7);
    drawableStack->addDrawable(brushSizeSliderBack, 5);
    drawableStack->addDrawable(infer, 5);
    drawableStack->addDrawable(erase, 5);
    drawableStack->addDrawable(paletteColorOutline, 5);
    drawableStack->addDrawable(paletteGrayOutline, 5);
    drawableStack->addDrawable(colorSelectTxt, 5);
    drawableStack->addDrawable(brushSizeSliderTxt, 6);
    drawableStack->addDrawable(inferTxt, 6);
    drawableStack->addDrawable(eraseTxt, 6);
    
    model = new ncnn::Net();
    
    ncnn::VulkanDevice* vkdev = ncnn::get_gpu_device();

    ncnn::VkAllocator* blob_vkallocator = new ncnn::VkBlobAllocator(vkdev);
    ncnn::VkAllocator* staging_vkallocator = new ncnn::VkStagingAllocator(vkdev);

    model->opt.blob_vkallocator = blob_vkallocator;
    model->opt.workspace_vkallocator = blob_vkallocator;
    model->opt.staging_vkallocator = staging_vkallocator;

    model->set_vulkan_device(0);
    model->opt.use_vulkan_compute = 1;
    
    model->load_param("ncnn.param");
    model->load_model("ncnn.bin");
}

void sfml::MainState::exit()
{
  delete model->opt.blob_vkallocator;
  model->opt.blob_vkallocator = nullptr;
  delete model->opt.staging_vkallocator;
  model->opt.staging_vkallocator = nullptr;
  delete model;
  model = nullptr;
	drawableStack->removeDrawables();
	delete drawableStack;
	drawableStack = nullptr;
}

void sfml::MainState::update()
{
    deltaTime = clock.restart().asSeconds();
    windowRatio.x = 1.0f;
    windowRatio.y = 1.0f;
    mouse.x = (cursor.x/scale)*windowRatio.x;
    mouse.y = (cursor.y/scale)*windowRatio.y;
    mouseReal.x = cursor.x*windowRatio.x;
    mouseReal.y = cursor.y*windowRatio.y;
    if (brushSize < 0)
    {
        brushSize = brushSizeMax;
    }
    if (brushSize > brushSizeMax)
    {
        brushSize = 0;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if ((mouse.x >= 0) && (mouse.y >= 0) && (mouse.x < pictureImg->getSize().x) && (mouse.y < pictureImg->getSize().y))
        {
            colorSelect->setFillColor(pictureImg->getPixel(mouse.x, mouse.y));
            brushColor = colorSelect->getFillColor();
        }
        else
        {
            brushColor = colorSelect->getFillColor();
        }
    }
    else
    {
        brushColor = colorSelect->getFillColor();
    }
    if ((mouse.x >= 0) && (mouse.y >= 0) && (mouse.x < pictureImg->getSize().x) && (mouse.y < pictureImg->getSize().y))
    {
        if (checkDraw())
        {
            drawLine(mousePrev, mouse);
            draw(mouse);
            pictureTex->loadFromImage(*pictureImg);
            //drawing = true;
        }
        else
        {
            drawing = false;
        }
    }
    if (drawing)
    {
        if ((paletteColorSpr->getPosition().x-32 <= mouseReal.x) &&
            (paletteColorSpr->getPosition().y <= mouseReal.y) &&
            (paletteColorSpr->getPosition().x+(window.x*.25f)+32 >= mouseReal.x) &&
            (paletteColorSpr->getPosition().y+32 >= mouseReal.y))
        {
            brushColor = colorConvert(sf::Vector3f((std::min(255.0f,std::max(0.0f,(mouseReal.x-(window.x-(window.x*.25f)+32))))/255.0f)*359.0f,1,1));
            colorSelect->setFillColor(brushColor);
        }
        if ((paletteGraySpr->getPosition().x-32 <= mouseReal.x) &&
            (paletteGraySpr->getPosition().y <= mouseReal.y) &&
            (paletteGraySpr->getPosition().x+(window.x*.25f)+32 >= mouseReal.x) &&
            (paletteGraySpr->getPosition().y+32 >= mouseReal.y))
        {
            gray = std::min(255.0f,std::max(0.0f,mouseReal.x-(window.x-(window.x*.25f)+32)));
            brushColor = sf::Color(gray,gray,gray);
            colorSelect->setFillColor(brushColor);
        }
        if ((brushSizeSliderBack->getPosition().x-32 <= mouseReal.x) &&
            (brushSizeSliderBack->getPosition().y <= mouseReal.y) &&
            (brushSizeSliderBack->getPosition().x+(window.x*.25f)+32 >= mouseReal.x) &&
            (brushSizeSliderBack->getPosition().y+32 >= mouseReal.y))
        {
            brushSizeSlider->setPosition(std::min(window.x-(window.x*.25f)+280,std::max(window.x-(window.x*.25f)+32,mouseReal.x)), brushSizeSlider->getPosition().y);
            brushSize = std::min(brushSizeMax,std::max(0,(int)(brushSizeMax*((mouseReal.x-(window.x-(window.x*.25f)+32))/256))));
            brushImg->create(brushSizeMax*2, brushSizeMax*2, sf::Color::Transparent);
            if (brushSize == 0)
            {
                brushImg->setPixel(brushSizeMax,brushSizeMax,sf::Color(128,128,128,128));
            }
            else
            {
                for (int i = -brushSize; i <= brushSize; i++)
                {
                    for (int ii = -brushSize; ii <= brushSize; ii++)
                    {
                        if ((i*i)+(ii*ii) <= brushSize*brushSize)
                        {
                            if ((brushSizeMax+i >= 0) && (brushSizeMax+ii >= 0) && (brushSizeMax+i < brushImg->getSize().x) && (brushSizeMax+ii < brushImg->getSize().y))
                            {
                                brushImg->setPixel(brushSizeMax+i,brushSizeMax+ii,sf::Color(128,128,128,128));
                            }
                        }
                    }
                }
            }
            brushTex->loadFromImage(*brushImg);
            brushSpr->setOrigin(brushSizeMax, brushSizeMax);
        }
        if ((infer->getPosition().x-32 <= mouseReal.x) &&
            (infer->getPosition().y <= mouseReal.y) &&
            (infer->getPosition().x+(window.x*.25f)+32 >= mouseReal.x) &&
            (infer->getPosition().y+32 >= mouseReal.y))
        {
            if (!inferMode)
            {
                inferMode = true;
                
                sf::Image* img = new sf::Image();
                img->create(pictureImg->getSize().x, pictureImg->getSize().y, pictureImg->getPixelsPtr());
                
                for (int x = 0; x != img->getSize().x; ++x)
                {
                    for (int y = 0; y != img->getSize().y; ++y)
                    {
                        sf::Color color = img->getPixel(x, y);
                        color.r = 255-color.r;
                        color.g = 255-color.g;
                        color.b = 255-color.b;
                        img->setPixel(x, y, color);
                    }
                }
                
                ncnn::Extractor ex = model->create_extractor();
                ncnn::Mat in = ncnn::Mat::from_pixels_resize(img->getPixelsPtr(), ncnn::Mat::PIXEL_RGBA2GRAY, img->getSize().x, img->getSize().y, 28, 28);
                ncnn::Mat out;
                ex.input("flatten_input", in);
                ex.extract("dense_2", out);
                
                for (int i = 0; i != 10; ++i)
                {
                    std::cout << i << '\t' << out.channel(0)[i] << std::endl;
                }
                std::cout << std::endl;
                
                delete img;
            }
        }
        if ((erase->getPosition().x-32 <= mouseReal.x) &&
            (erase->getPosition().y <= mouseReal.y) &&
            (erase->getPosition().x+(window.x*.25f)+32 >= mouseReal.x) &&
            (erase->getPosition().y+32 >= mouseReal.y))
        {
            if (!eraseMode)
            {
                eraseMode = true;
                drawableStack->removeDrawable(pictureSpr);
                delete pictureSpr;
                delete pictureTex;
                delete pictureImg;
                pictureImg = new sf::Image();
                pictureImg->create(dimension,dimension,sf::Color::White);
                pictureTex = new sf::Texture();
                pictureTex->loadFromImage(*pictureImg);
                pictureSpr = new sf::Sprite(*pictureTex);
                pictureSpr->setScale(scale,scale);
                drawableStack->addDrawable(pictureSpr, 2);
            }
        }
    }
    else
    {
        inferMode = false;
        eraseMode = false;
    }
    mousePrev = mouse;
    if ((mouse.x >= 0) && (mouse.y >= 0) && (mouse.x < pictureImg->getSize().x) && (mouse.y < pictureImg->getSize().y))
    {
        brushSpr->setPosition(mouseReal);
    }
    else
    {
        brushSpr->setPosition(-100*scale, -100*scale);
    }
    if ((colorSelect->getFillColor().r == colorSelect->getFillColor().g) &&
        (colorSelect->getFillColor().r == colorSelect->getFillColor().b) &&
        (colorSelect->getFillColor().g == colorSelect->getFillColor().b))
    {
        gray = (1-((colorSelect->getFillColor().r+128)/255))*255;
        colorSelectTxt->setFillColor(sf::Color(gray,gray,gray));
    }
    else
    {
        colorSelectTxt->setFillColor(sf::Color::Black);
    }
}

void sfml::MainState::drawLine(sf::Vector2f point1, sf::Vector2f point2)
{
    sf::Vector2i point;
    int dx, dy, dx1, dy1, px, py, xe, ye;
    if ((point1.x < 1) || (point1.y < 1))
    {
        return;
    }
    dx = point2.x-point1.x;
    dy = point2.y-point1.y;
    dx1 = fabs(dx);
    dy1 = fabs(dy);
    px = 2*dy1-dx1;
    py = 2*dx1-dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            point.x = point1.x;
            point.y = point1.y;
            xe = point2.x;
        }
        else
        {
            point.x = point2.x;
            point.y = point2.y;
            xe = point1.x;
        }
        draw(sf::Vector2f(point.x, point.y));
        for (int i = 0; point.x < xe; i++)
        {
            point.x = point.x+1;
            if(px < 0)
            {
                px = px+(2*dy1);
            }
            else
            {
                if (((dx < 0) && (dy < 0)) || ((dx > 0) && (dy>0)))
                {
                    point.y = point.y+1;
                }
                else
                {
                    point.y = point.y-1;
                }
                px = px+(2*(dy1-dx1));
            }
            draw(sf::Vector2f(point.x, point.y));
        }
    }
    else
    {
        if (dy >= 0)
        {
            point.x = point1.x;
            point.y = point1.y;
            ye = point2.y;
        }
        else
        {
            point.x = point2.x;
            point.y = point2.y;
            ye = point1.y;
        }
        draw(sf::Vector2f(point.x, point.y));
        for(int i = 0; point.y < ye; i++)
        {
            point.y = point.y+1;
            if (py <= 0)
            {
                py = py+(2*dx1);
            }
            else
            {
                if (((dx < 0) && (dy < 0)) || ((dx > 0) && (dy > 0)))
                {
                    point.x = point.x+1;
                }
                else
                {
                    point.x = point.x-1;
                }
                py = py+(2*(dx1-dy1));
            }
            draw(sf::Vector2f(point.x, point.y));
        }
    }
}

void sfml::MainState::draw(sf::Vector2f point)
{
    if (brushSize == 0)
    {
        if ((point.x >= 0) && (point.y >= 0) && (point.x < pictureImg->getSize().x) && (point.y < pictureImg->getSize().y))
        {
            pictureImg->setPixel(point.x,point.y,brushColor);
        }
    }
    else
    {
        for (int i = -brushSize; i <= brushSize; i++)
        {
            for (int ii = -brushSize; ii <= brushSize; ii++)
            {
                if ((i*i)+(ii*ii) <= brushSize*brushSize)
                {
                    if ((point.x+i >= 0) && (point.y+ii >= 0) && (point.x+i < pictureImg->getSize().x) && (point.y+ii < pictureImg->getSize().y))
                    {
                        pictureImg->setPixel(point.x+i,point.y+ii,brushColor);
                    }
                }
            }
        }
    }
}

sf::Color sfml::MainState::colorConvert(sf::Vector3f color)
{
	int i = 0;
	float f = 0, p = 0, q = 0, t = 0;
	sf::Color colorNew;
	if (color.y == 0)
    {
		colorNew.r = color.z;
		colorNew.g = color.z;
		colorNew.b = color.z;
    }
    else
    {
        color.x /= 60;
        i = floor(color.x);
        f = color.x-i;
        p = color.z*(1.0f-color.y);
        q = color.z*(1.0f-(color.y*f));
        t = color.z*(1.0f-(color.y*(1.0f-f)));
        switch (i)
        {
        case 0:
            colorNew.r = color.z*255;
            colorNew.g = t*255;
            colorNew.b = p*255;
            break;
        case 1:
            colorNew.r = q*255;
            colorNew.g = color.z*255;
            colorNew.b = p*255;
            break;
        case 2:
            colorNew.r = p*255;
            colorNew.g = color.z*255;
            colorNew.b = t*255;
            break;
        case 3:
            colorNew.r = p*255;
            colorNew.g = q*255;
            colorNew.b = color.z*255;
            break;
        case 4:
            colorNew.r = t*255;
            colorNew.g = p*255;
            colorNew.b = color.z*255;
            break;
        case 5:
            colorNew.r = color.z*255;
            colorNew.g = p*255;
            colorNew.b = q*255;
            break;
        }
    }
    return colorNew;
}
