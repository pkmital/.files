# Introduction

This repo contains a single file in `data/data.npy`, a 2-D array (`n_observations, n_tasks`) with `n_observations` = 20000 and `n_tasks` = 2.  Please complete the following tasks only using the numpy and tensorflow libraries (i.e. no other imports). For TensorFlow, please use < 2.0.

# Task 1: Input Data Pipeline

Your task is to demonstrate how you would write a TensorFlow data input pipeline (https://www.tensorflow.org/api_docs/python/tf/data) and session which shuffles and batches the data.  For instance, given a `batch_size` of 32, demonstrate how your input pipeline would iterate over the original dataset, producing random mini-batches of data.

# Task 2: Balanced Data Pipeline

The original distribution of the data has a heavily oversampled class.  Now demonstrate how your input pipeline could produce balanced batches of data, ensuring it still provides minibatches of data from all observations (i.e. do not remove any of the data). 

Demonstrate the resulting distribution of your data produced by your data pipeline.

Comment on whether your solution is optimized for compute, memory, or both. Does it incur these costs before or during the input pipeline? And does it incur these costs once, or each time the data pipeline is called?

# Task 3: Online Balanced Data

Now find a way to balance the mini batches produced by your input data pipeline, but in an "online" way.  In other words, you are required to use the same amount of memory as the original data, and not store additional data. 

Again, demonstrate the resulting distribution of your data produced by your data pipeline.

Note that if your solution for Task 2 already does online balance sampling, then you can skip this task.

# Task 4: Tests

Document your code and write tests using standard python tools (e.g. unittest, mock, py.test, tox, etc...), software environment tools (e.g. Docker, virtualenv, etc...) and demonstrate how your code would be tested if integrated into a larger project.
