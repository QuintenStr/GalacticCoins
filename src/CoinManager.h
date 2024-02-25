#pragma once

#include "AssetManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"

#include <set>
#include <string>
#include <utility>

class CoinManager
{
public:
    static CoinManager& getInstance();
    void                init();
    void                deleteCoinAndMakeNewOne(const std::string& id);

private:
    CoinManager();
    void                makeCoin();
    std::pair<int, int> generateRandomCoordinates();
    bool                isOverlapping(int x, int y);

    int                           m_coinCounter;
    std::set<std::pair<int, int>> m_coinPositions; // Stores the positions of all parts of all coins
};
