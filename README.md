# Files2CPP
Also known as Files to C++. Its a work in progress.

## What is this abomination?
Files2CPP (Files to C++) is an example of a idea that happened at the spur of the moment. While developing a game, I thought: "Wouldn't it be cool if I could store all my game resources in a `.dll` or `.exe` file?"

Hence, I went ahead and did some research. The [most common and the most platform independent way](http://stackoverflow.com/questions/6785214/how-to-embed-a-file-into-an-executable-file), was to get your file in hex, and then paste it into an array lying in a header file. (`unsigned int` or `unsigned char` are good for this)

That is good and all, but isn't that a little problematic? I **am** making a game, so that means I would have tons upon tons of resources to throw into my header file. `#include`ing this header file will invoke a nightmarish disaster that will be unleashed upon the living realm.

So I looked for a tool that was able to automatically place all of these data into a function, which acted as a switch, which dynamically allocates the resource only when it is required.

Of course, I was either blind, or Google had failed me. The only closest alternative I could find was [this](http://stackoverflow.com/questions/4864866/c-c-with-gcc-statically-add-resource-files-to-executable-library/4864879#4864879).

Hence, the monkey behind the screen decided to do something - to make his own tool to do this. Hence, the abnormality we call Files2CPP.

## Why not use XXXX instead?
You: After all, it's better than your crappy coded crap.

Good question!

## Where do I download?
If it isn't in the [releases](https://github.com/jameshi16/Files2CPP/releases), you'll have to compile it yourself.

If you have C++17 on your compiler already (lucky you!), change all the code to support that! If you do not have C++17 on your compiler, then compile it with the boost library, namely the `system` library and the `filesystem` library.

## Y U NO USE C++17 D:<
GCC 6.3.0 still calls it c++0z :^(

I'll upgrade when:
1. GCC updates and fully supports C++17
2. I'm not busy
3. My computer isn't on fire


## License
I have no idea what I'm doing, so here's the [Apache License](https://github.com/jameshi16/Files2CPP/blob/master/LICENSE). Essentially, if the lawyer in me is correct, you can do anything you want with the code. I think. Probably.
