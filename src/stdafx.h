#pragma once

#if _DEBUG && true ///< Set true for leak detection with file names and line numbers
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new     DBG_NEW

#endif

#include "Debug.hpp"
#include "targetver.h"
#include "tileson.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
