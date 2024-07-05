# Part A

In the early quests of the game, the town's tavern is advertising exciting new flavours for their soups. The cook has promised 500 silver pieces for each novel ingredient on their list gathered. Though appearing generous for ingredient gathering in the nearby forest, the party cleric noticed that each ingredient requires crawling between poison bushes. The party rogue has the smallest build and will likely be able to crawl through the bush without being scratched too badly - but will surely be afflicted. Once the poison takes hold, each step taken will passively do a fixed amount of damage. The cleric's healing spell can only be casted once the party rogue has exited the narrow space. Since each casting of the healing spell requires expensive magical materials, you will have to find the route that reaches the exit in the least possible steps. 

An example of the forest pathways are shown below, where each edge represents a step and each node represents a location the rogue can step to:
IMAGE
The forest layout above would be represented by the input to your program:
8
11
0
5
0 1
0 3
1 2
1 4
1 6
2 4
3 6
4 6
4 7
5 7
6 7

Where:

    the first line (8) is the number of locations the rogue can possibly step, 

    the following line (11) is the number of connections between locations,

    the third line (0) is the location the rogue starts at,

    the fourth line (5) is the location the rogue exits the space and is in range to be healed, and

    all following lines are pairs, indicating an undirected connection between the two locations.

You may assume the list is sorted numerically by the first location and then (where the first location is equal) by the second location. You may also assume locations are always equal to or greater than 0 and numbered less than the number of locations.

The output should be the damage taken - assuming one point of damage is taken per step.
# Part B

After hearing the ease with which our protagonists were able to deal with the poison forest, a local merchant staying at the tavern asks if the party would be interested in a challenge. The merchant had come into possession of a number of lair maps that showed the locations of all the traps in each lair. The merchant confessed they did not have any non-mercantile skills but had managed to purchase information on how to disarm each trap. After discussing the skills the party held, the merchant explained the cost of the materials they'd need to disarm each trap they had the skills to disarm and marked this total cost on each pathway. To preserve the most treasure, you'll need to find the cheapest path through the lair.

Here is an example of one of these maps:
IMAGE
This map would be represented using the following format:
8
11
0
5
0 1 5
0 3 4
1 2 3
1 4 6
1 6 8
2 4 3
3 6 2
4 6 4
4 7 8
5 7 4
6 7 6

Which is the same as the format for Part A, but each edge specified includes an additional third number describing the cost of disarming the trap. You may also assume that all costs will be non-negative.
Part C

To reward the adventurers for their extensive help connecting the merchant with their extensive treasures, they share their own prior connections. The documents they share detail artisans who will offer ongoing services for a particular price - each artisan on the map is able to create one material from another and reverse the process. Each document is limited to the connections for a particular set of materials which are related in some way. Since the general store merchants offer a new daily special periodically, having a network which allows the creation of all materials from any material in the documents will save a lot of money in future adventures. You will have to find the gold you'll need to build the network which allows any material to be reached from any other material in the document.

Here is an example of one of these documents:
IMAGE
This document would be represented using the following format:
8
11
0
0 1 5
0 3 4
1 2 3
1 4 6
1 6 8
2 4 3
3 6 2
4 6 4
4 7 8
5 7 4
6 7 6

Which is similar to the format for Part B, but excludes the fourth line that would normally specify the final destination.

The output will be the minimum cost required to pay across all artisans such that each material in the document can be made from any other material.
# Part D

The final battle with Balgor, the Ruler of the Delve requires venturing into the eponymous mythic delves. Mythic delves are typically reserved for the strongest of adventurers, as each room applies a further unique condition which increases the amount of damage done. Those who came before you have managed to map the damage multiplier that each room applies, and have sold these precious maps to you for a hefty sum. Since one of Balgor's minions lies at the end of each mythic delve, you will have to reach the end of the delve with the lowest multiplier to have any chance of besting them.

The game uses the mathematical floor of the calculated multiplier when calculating the final damage multiplier (as this multiplier is displayed to the player as a whole number) - however this is only applied to the final value, intermediate calculations retain the fractional elements.

Here is an example of a map of a mythic delve:
IMAGE
This map would be given in the following format:
8
10
0
7
0 1 20 
0 2 30
1 2 10
2 3 60
3 4 80
3 6 20
4 5 20
5 6 10
5 7 90
6 7 120

This is similar to the format of Part A and B, but the weight of each edge is the percentage increase (e.g. a value of 20 implies a 20% increase in the damage taken in each subsequent room). For this problem, successive rooms multiply by the weight of the previous rooms, so in the above map, a path which takes the edge between 0 and 1, followed by the edge between 1 and 2, would apply a total increase of 32%.

The output should be the final percentage increase (without the percent sign) with any fractional part of the percentage omitted.
