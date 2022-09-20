#ifndef DRAWABLE_STACK_HPP
#define DRAWABLE_STACK_HPP

#include <map>
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>

class DrawableStack : public sf::Drawable
{
public:
    DrawableStack() :
		sf::Drawable()
    {
        
    }
    void addDrawable(sf::Drawable* drawable, unsigned int depth)
    {
        std::map<unsigned int, std::vector<sf::Drawable*> >::iterator iter = drawables.find(depth);
        if (iter == drawables.end())
        {
            drawables[depth] = std::vector<sf::Drawable*>();
            iter = drawables.find(depth);
        }
        iter->second.push_back(drawable);
    }
    bool removeDrawable(sf::Drawable* drawable, unsigned int depth)
    {
        std::map<unsigned int, std::vector<sf::Drawable*> >::iterator iter = drawables.find(depth);
        if (iter != drawables.end())
        {
            for (unsigned int i = 0; i != iter->second.size(); ++i)
            {
                if (iter->second[i] == drawable)
                {
                    iter->second.erase(iter->second.begin()+i);
                    return true;
                }
            }
        }
        return false;
    }
    bool removeDrawable(sf::Drawable* drawable)
    {
        for (std::map<unsigned int, std::vector<sf::Drawable*> >::iterator iter = drawables.begin(); iter != drawables.end(); ++iter)
        {
            if (removeDrawable(drawable,iter->first))
            {
                return true;
            }
        }
        return false;
    }
	void removeDrawables()
	{
        for (std::map<unsigned int, std::vector<sf::Drawable*> >::const_iterator iter = drawables.begin(); iter != drawables.end(); ++iter)
        {
            for (std::vector<sf::Drawable*>::const_iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
            {
                delete *iter2;
            }
        }
		drawables.clear();
	}
protected:
    void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
    {
        for (std::map<unsigned int, std::vector<sf::Drawable*> >::const_iterator iter = drawables.begin(); iter != drawables.end(); ++iter)
        {
            for (std::vector<sf::Drawable*>::const_iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
            {
                renderTarget.draw(**iter2,renderStates);
            }
        }
    }
private:
    std::map<unsigned int, std::vector<sf::Drawable*>> drawables;
};

#endif
