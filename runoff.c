#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;
// Number of max and min votes
int mostvotes, leastvotes;
// Winner name
string winner;
string loser;

// isTie boolean
bool isTie = false;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i ++) // Loop for checking name
    {
        if (strcmp(candidates[i].name, name) == 0)  // Check name is correct
        {
            preferences[voter][rank] = i; // Add vote
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        int j = 0; // rank

        while (candidates[preferences[i][j]].eliminated == true) // Checks if candidate has been eliminated
        {
            j++;
        }

        candidates[preferences[i][j]].votes++; // if not eliminated adds vote
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++) // loops through candidates votes amounts
    {
        if (candidates[i].votes >= mostvotes) // checks if votes are max num of votes
        {
            mostvotes = candidates[i].votes;
            if (mostvotes > (voter_count / 2)) // checks if votes are greater than half the votes
            {
                winner = candidates[i].name; // if greater prints winner
                printf("Winner: %s\n", winner);
                return true;
            }
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    for (int i = 0; i < candidate_count; i++) // loops through candidates
    {
        leastvotes = candidates[0].votes; // sets leastvotes to first candidate

        if (candidates[i].eliminated == false
            && leastvotes > candidates[i].votes) // check not eliminated and if votes are the lowest
        {
            leastvotes = candidates[i].votes; // if less changes leastvotes to candidates
        }
    }
    return leastvotes; // returns candidate
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    min = leastvotes;
    int count = 0;
    int eliminated = 0;

    for (int i = 0; i < candidate_count; i++) // loop through candidates
    {
        if (!candidates[i].eliminated)
        {
            eliminated ++; // adds to eliminated counter
        }
        if (candidates[i].votes == min) // check elimination bool and if their votes equal least votes
        {
            count ++; // adds to count
        }
    }

    if (eliminated == count) // checks if count equals the amount of candidates left
    {
        isTie = true;
        //("It's a tie!\n"); // Test
        return true; // returns true if it is a tie
    }
    else
    {
        return false; // returns false if it is not
    }
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++) // loop through candidates
    {
        if (candidates[i].eliminated == false
            && candidates[i].votes == min) // checks elimination status and checks their votes equal least votes
        {
            candidates[i].eliminated = true; // eliminates candidate
            printf("eliminated: %s\n", candidates[i].name);
        }
    }
    return;
}
