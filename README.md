
# Show Recommender

We created a site that would allow us to create recommendations for people. 

Using C++ we parsed through our dataset and created an adjacency list, with our weigted edges being those of shared features.

We gave some options for the user:
If they want to recommendations related to 2 shows, they can use Djikstras or A star to be able to find a path and see what shows come up along the way.

If they just want a general filtering system, we used a priority queue to help sort and give the shows with the best result.

TO RUN:
Download the .cpp and .h files.
Download the dataset.
ENSURE ALL FILES IN DATA SET IS LOCATED WHERE YOUR EXECUTABLE IS.
Run program through main.cpp

Dataset used: https://www.kaggle.com/datasets/denizbilginn/tv-shows
