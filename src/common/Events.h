////////////////////////////////////////////////////////////////////////////////
// Super Pac-Man clone
//
// Copyright (c) 2020-2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#ifndef SUPERPACMAN_GAMEEVENTS_H
#define SUPERPACMAN_GAMEEVENTS_H

namespace spm {
    /**
     * @brief Events that take place during gameplay
     */
    enum class GameEvent {
        FrightenedModeBegin, //!< Occurs when pacman eats an energizer pellet
        FrightenedModeEnd,   //!< Occurs when the frightened mode timesout
        ChaseModeBegin,      //!< Occurs when scatter mode expires
        ScatterModeBegin,    //!< Occurs when chase mode expires
        SuperModeBegin,      //!< Occurs when pacman eats a Super pellet
        SuperModeEnd,        //!< Occurs when super pellet effects wore off
    };
}

#endif //SUPERPACMAN_GAMEEVENTS_H
