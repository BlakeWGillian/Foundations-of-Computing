## Part A

# Implement the Jarvis' March algorithm to compute the convex hull. Find the leftmost point. Then iterate over the remaining points, selecting the most counterclockwise point relative to the current point until you come back to the starting point.
Requirements

    Data Structure: Implement a doubly linked list to store the points of the convex hull. Each node should store the x and y coordinates of a point and pointers to the next and previous nodes. 

    Input Format: The input will be a CSV file where the first line indicates the total number of points. Subsequent lines represent points with their x and y coordinates separated by a space (e.g., x y).

    Output Format: Your program should output two sequences of points forming the convex hull: one in clockwise order and the other in counterclockwise order. This should be done by traversing your doubly linked list to print the points in both clockwise and counterclockwise order, in both cases starting from the first point added to the hull. This may require starting from a specific point and moving in one direction for clockwise and the opposite direction for counterclockwise. This should be output to the console (stdout).

## Part B

# Implement the Graham's Scan algorithm to compute the convex hull. Find the bottom-most point, sort the points, and proceed with the scan, pushing and popping points from the stack (which will be your doubly linked list in this case) to ensure you maintain a convex hull throughout.
Requirements

    Data Structure: Implement a stack with doubly linked lists. Utilize this stack in Graham's Scan algorithm to store the convex hull points. Each node should store the x and y coordinates of a point and pointers to the next and previous nodes. 

    Input Format: The input will be a CSV file where the first line indicates the total number of points. Subsequent lines represent points with their x and y coordinates separated by a space (e.g., x y).

    Output Format: Your program should output two sequences of points forming the convex hull: one in clockwise order and the other in counterclockwise order. This should be done by traversing your doubly linked list to print the points in both clockwise and counterclockwise order, in both cases starting from the first point added to the hull. This may require starting from a specific point and moving in one direction for clockwise and the opposite direction for counterclockwise. This should be output to the console (stdout).

## Part C

# Evaluate both algorithms through experimental analysis by quantifying the total basic operations across various input scales and configurations. Consider creating input sets of at least three distinct sizes, each under three differing distribution conditions: random, points on a circle, and random points contained within a set of points making up a simple hull.

You should use the following basic operations for each algorithm:

    Jarvis' March - A comparison between the angles of points.

    Graham's Scan - A comparison between the angles of points during the sort.

Reporting: Write a report including a discussion on the choice of data structure, the experimental evaluation, and conclusions drawn from the comparisons. Include any assumptions or simplifications made in your implementations.
