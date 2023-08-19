# PlantsvsZombies_TwoPlayerEdition
## Introduce
This program is based on the C++ two-player game developed by EasyX Graphics Library. The game is written in the style of Dr. Zombie's Revenge in the original Plants vs. Zombies, but there are still many unrealized functions. Users can control the placement of plants with the mouse or the keyboard Zombie placement
## Instructions for use
### Plants
The operation of plants is consistent with the original version. The placement of plants is controlled by the left button of the mouse. The difference from the original version is that there is no ghost hint when placing them, and they may be placed in the wrong place when placing them.
Type of plant:
**1. Cabbage**
HP: 6
Attack: 40
**Attack speed is twice the original version**
**2. Watermelon**
HP: 6
Attack: 80
**Attack speed is twice the original version**
**3. Corn**
HP: $\infty$
Attack: 1800
**Ash plants, only placed in pots**
**4. Ice Mushroom**
HP: $\infty$
**Attack: 100**
ashes plant
**5. Chilli**
HP: $\infty$
Attack: 1800
ashes plant
**6. Flower Pot**
HP: 6
### Zombies
Zombies are placed through the keyboard, and the up, down, left, and right keys can control which row the zombies should be placed in
**1. Ordinary Zombie**
Place key: z/Z
HP: 200
Attack: 1
Speed: 1
**2. Barricade Zombie**
Place key: x/X
HP: 400
Attack: 1
Speed: 1
**3. Tietong Zombie**
Place key: c/C
HP: 1000
Attack: 1
Speed: 1
**4. Rugby Zombie**
Place key: v/V
HP: 1800
Attack: 1
**Speed: 2**
**5. Catapult Zombie**
Place key: b/B
HP: 1200
**Attack: 2**
**Speed: 0**
**It will only attack the plants in your last row, it will not move forward, nor will it crush the plants**
**6. Clown Zombie**
Place key: n/N
HP: 400
Attack: 1
Speed: 1
**Explodes after death, destroying the three plants that are close to (to the extent that they can be eaten) in front of the clown**
## QA
Q: Why drag a plant to an illegal position (such as the sky, the moon, behind the last row) and click to disappear?
A: Because the program does not do a withdrawal operation when processing cards, that is, as long as the card is taken out, it must be planted, otherwise you can click anywhere outside the planting area to destroy the plant (also to avoid a crash in an extreme situation: planting on the flowerpot Plants, but the plants in your hands cannot be planted, put back, or destroyed)
Q: Why can't I place plants in the ninth row?
A: The ninth row is for zombies, plants are not allowed
Q: Why sometimes the corn cannon can't hit the catapult zombie?
A: The mystery judgment of Catapult Zombie can only be exploded by placing flower pots in the front row (the 8th row from back to front).
## Acknowledgments
**Thanks to [Ooil9965](https://github.com/Ooil9965) for the material! ! ! **