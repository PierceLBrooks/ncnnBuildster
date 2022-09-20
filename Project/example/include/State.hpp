#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "DrawableStack.hpp"

namespace sfml
{
    class State
    {
        public:
            State();

            virtual void enter() = 0;

            virtual void exit() = 0;

            virtual void update() = 0;

            DrawableStack* drawableStack;
    };
}

#endif // STATE_HPP
