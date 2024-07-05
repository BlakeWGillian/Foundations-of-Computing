'''
Inside solve_FoCdle(), each cycle of the inner loop generates another new_guess, and then checks whether or not it is compliant with the current all_info. But it isn't allowed to do that endlessly, and after getting as many as GCOUNT_MAX candidate guesses from create_guess() without finding a perfect one, it is required to commit to the last candidate, new_guess, even though it can't be a solution. That guess is then applied via set_colors() (and counted towards the guess score in nguesses), in the expectation that it will likely add new information (via new_info) to the growing set of overall restrictions (the list all_info).

Each iteration of the outer loop tries to find a good guess, then tests it as an "official" guess, and gets back another row of "colored boxes" to extend all_info with further information. The anticipation is that as all_info gets more and more restrictive, eventually a guess should be arrived at that is all_green(), and thus solves the FoCdle.


In this third task you are to write the function:


def passes_restrictions(guess, all_info):

    Tests a `guess` equation against `all_info`, a list of known restrictions, 

    one entry in that list from each previous call to set_colors(). Returns 

    True if that `guess` complies with the collective evidence imposed by 

    `all_info`; returns False if any violation is detected. Does not check the 

    mathematical accuracy of the proposed candidate equation.

    
'''


GREEN = "green"
YELLO = "yellow"
GREYY = "grey"

def passes_restrictions(guess, all_info):
    '''
    Input: A FoCdle 'guess', and a two dimensional list 'all_info' of color info;
    Output: False if the guess ignores information from all_info, else True
    '''
    # For every set_colors block in all_info
    for stage in all_info:
        # Make a temporary guess, so that it can be changed for each stage.
        mod_guess = guess
        # Make sure that order the information is evaluated is: Gre, Ye, Gry
        stage = sorted(stage, key=lambda a: 
                       ('z' if a[2] == 'grey' else a[2], a[0]))
        for position in stage:
            if position[2] == "green":
                if mod_guess[position[0]] != position[1]:
                    return False
                else:
                    # Replace the green character with an x for the yellow eval.
                    mod_guess = mod_guess[0:position[0]] + "x" + \
                                mod_guess[position[0] + 1:]
            elif position[2] == "yellow":
                if position[1] not in mod_guess or\
                    mod_guess[position[0]] == position[1]:
                    return False
                else:
                    # replace the first character with an x for other yellow evals.
                    mod_guess = mod_guess.replace(position[1], "x", 1)
            else:
                # If the character is grey, and in the guess.
                if position[1] in mod_guess:
                    return False
    return True
