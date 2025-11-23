#!/bin/bash
set -e

echo "Building Warzone project..."
g++ -std=c++17 -Wall \
orders/orders.cpp \
player/player.cpp \
map/map.cpp \
cards/cards.cpp \
gameEngine/gameEngine.cpp \
processor/Command.cpp \
processor/CommandProcessor.cpp \
processor/FileCommandProcessorAdapter.cpp \
playerStrategies/playerStrategies.cpp \
playerStrategies/playerStrategiesDriver.cpp \
map/mapDriver.cpp \
gameEngine/gameEngineDriver.cpp \
processor/CommandProcessorDriver.cpp \
orders/ordersDriver.cpp \
player/playerDriver.cpp \
cards/cardsDriver.cpp \
logObserver/LoggingObserver.cpp \
logObserver/loggingObserverDriver.cpp \
main/mainDriver.cpp \
-o mainDriver

echo "Build complete. Running program..."
./mainDriver