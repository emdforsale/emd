Empirical Mode decomposition(EMD) uses in number of applications, such as: Financial applications, image processing, biomedical applications etc.
More info: 

[Hilbert–Huang_transform](https://en.wikipedia.org/wiki/Hilbert–Huang_transform)

[Empirical_Mode_Decomposition_-_an_introduction](https://www.researchgate.net/publication/221534245_Empirical_Mode_Decomposition_-_an_introduction)

For our own purpose we realized this application in C language. First version of algorighm took too much memory(around 50 bytes per data tick + overhead). Then we decided to implement another version, applicable for embedded devices with very small amount of memory. Second version uses only 8 bytes per count + overhead(around 2-3kBytes). And of course it is slower. We used this alghorithm with very tiny microcontroller with 48KB RAM, and we were able to process 1024 data ticks. 

All memory used by algorithm placed into context(excluding local variables). 

For first version of algorithm see
how context looks like [here](https://github.com/emdforsale/emd/blob/master/inc/spline.h), 
for second [here](https://github.com/emdforsale/emd/blob/master/inc/spline_light.h).

Second version has a little bit differ results, you can check it [here](http://htmlpreview.github.io/?https://github.com/emdforsale/emd/blob/master/bin/wasm_app.html) (There is emscripten application with our libraries for estimating results and how it works). 
But it is not critical for the most application.

How to use both libraries you can see [here](https://github.com/emdforsale/emd/blob/master/src/main.cpp)

These examples contain datasets, collected from real people. Collection was done using Si1102 sensors from wrists of real people(pulse wave) with length 1-2 seconds. 

We can provide first and/or second version of algorithm as binary library by your request. Just contact us by e-mail: emdforsale@gmail.com
