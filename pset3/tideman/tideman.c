#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

bool createsCycle(int target, int start);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] == preferences[j][i]) {
                continue;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
            }
            else
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
            }
            pair_count++;
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int swapped = 0;
        // get the "strength" of the victory by the difference of preferences
        // sort the pairs using the bubble sort algorithm
        for (int j = 0; j < pair_count - i; j++)
        {
            int winnerPrefs = preferences[pairs[j].winner][pairs[j].loser];
            int loserPrefs = preferences[pairs[j].loser][pairs[j].winner];
            int difference = winnerPrefs - loserPrefs;

            int nextWinnerPrefs = preferences[pairs[j + 1].winner][pairs[j + 1].loser];
            int nextLoserPrefs = preferences[pairs[j + 1].loser][pairs[j + 1].winner];
            int nextDifference = nextWinnerPrefs - nextLoserPrefs;

            if (difference < nextDifference)
            {
                pair tmpPair = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = tmpPair;
                swapped = 1;
            }
        }
        if (!swapped)
        {
            break;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // for each winner in the pairs array, we should create an edge that points to the loser
    // but before creating an edge in the graph, we should check if this edge
    // would create a cycle that points to the current winner
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        if (!createsCycle(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }
    return;
}

// checks if the locked matrix has a path that points
// from start to targed
bool createsCycle(int target, int start)
{
    if (start == target)
        return true;

    bool foundCycle = false;
    for (int i = 0; i < candidate_count; i++)
    {
        // follow everyone that start has beaten to see if they lead to target
        if (locked[start][i])
        {
            if(createsCycle(target, i))
            {
                foundCycle = true;
            }
        }
    }

    // if it didn't beat anyone, it definetly won't create a cycle
    return foundCycle;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool lost = false;
        for (int j = 0; j < candidate_count; j++)
        {
            // if it's locked, dude lost. Check the next one
            if (locked[j][i])
            {
                lost = true;
                break;
            }
        }
        if (!lost)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
