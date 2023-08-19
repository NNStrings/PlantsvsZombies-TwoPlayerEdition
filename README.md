# PlantsvsZombies_TwoPlayerEdition
## Introduce
This program is based on the C++ two-player game developed by EasyX Graphics Library. The game is written in the style of Dr. Zombie's Revenge in the original Plants vs. Zombies, but there are still many unrealized functions. Users can control the placement of plants with the mouse or the keyboard Zombie placement
## Instructions for use
### Plants
The operation of plants is consistent with the original version. The placement of plants is controlled by the left button of the mouse. The difference from the original version is that there is no ghost hint when placing them, and they may be placed in the wrong place when placing them.
Type of plant:<br>
**1. Cabbage**<br>
HP: 6<br>
Attack: 40<br>
**Attack speed is twice the original version**<br>
**2. Watermelon**<br>
HP: 6<br>
Attack: 80<br>
**Attack speed is twice the original version**<br>
**3. Corn**<br>
HP: $\infty$<br>
Attack: 1800<br>
**Ash plants, only placed in pots**<br>
**4. Ice Mushroom**<br>
HP: $\infty$<br>
**Attack: 100**<br>
ashes plant<br>
**5. Chilli**<br>
HP: $\infty$<br>
Attack: 1800<br>
ashes plant<br>
**6. Flower Pot**<br>
HP: 6<br>
### Zombies<br>
Zombies are placed through the keyboard, and the up, down, left, and right keys can control which row the zombies should be placed in<br>
**1. Normal Zombie**<br>
Place key: z/Z<br>
HP: 200<br>
Attack: 1<br>
Speed: 1<br>
**2. Barricade Zombie**<br>
Place key: x/X<br>
HP: 400<br>
Attack: 1<br>
Speed: 1<br>
**3. Bucket Zombie**<br>
Place key: c/C<br>
HP: 1000<br>
Attack: 1<br>
Speed: 1<br>
**4. Football Zombie**<br>
Place key: v/V<br>
HP: 1800<br>
Attack: 1<br>
**Speed: 2**<br>
**5. Catapult Zombie**<br>
Place key: b/B<br>
HP: 1200<br>
**Attack: 2**<br>
**Speed: 0**<br>
**It will only attack the plants in your last row, it will not move forward, nor will it crush the plants**<br>
**6. Clown Zombie**<br>
Place key: n/N<br>
HP: 400<br>
Attack: 1<br>
Speed: 1<br>
**Explodes after death, destroying the three plants that are close to (to the extent that they can be eaten) in front of the clown**<br>
## QA
Q: Why drag a plant to an illegal position (such as the sky, the moon, behind the last row) and click to disappear?<br>
A: Because the program does not do a withdrawal operation when processing cards, that is, as long as the card is taken out, it must be planted, otherwise you can click anywhere outside the planting area to destroy the plant (also to avoid a crash in an extreme situation: planting on the flowerpot Plants, but the plants in your hands cannot be planted, put back, or destroyed)<br>
Q: Why can't I place plants in the ninth row?<br>
A: The ninth row is for zombies, plants are not allowed<br>
Q: Why sometimes the corn cannon can't hit the catapult zombie?<br>
A: The mystery judgment of Catapult Zombie can only be exploded by placing flower pots in the front row (the 8th row from back to front).<br>
## Acknowledgments
**Thanks to [Ooil9965](https://github.com/Ooil9965) for the material! ! !**