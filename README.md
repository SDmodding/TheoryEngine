<p align="center">
    <img width="75%" src="https://github.com/user-attachments/assets/9cfd0023-625e-4292-81b1-3a2f9b394cdd">
    <br>
    <img alt="GitHub Created At" src="https://img.shields.io/github/created-at/SDmodding/TheoryEngine?color=baa6ff">
    <img alt="GitHub contributors" src="https://img.shields.io/github/contributors/SDmodding/TheoryEngine?color=baa6ff">
    <img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/SDmodding/TheoryEngine?color=baa6ff">
    <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/SDmodding/TheoryEngine?color=baa6ff">
</p>

##

<p align="center">
    This is a <b>work in progress</b> reverse-engineered reimplementation of the <u>Sleeping Dogs: Definitive Edition (Engine)</u> with a help of the debug symbols (PDB file) that was shipped with the release of Steam version. We use only headers so it can be easily embedded and included to any side project. The goal is to provide a functional codebase that resemble original engine.
    <br><br>
    <b><u>Resources:</u></b><br>
    <a href="https://mega.nz/file/fK5SWARD#1fAWkxAHaKCIMDaJ5XAQKvjs6gK4RCQo5ZlvvtHWtVw">Game Executable</a> • <a href="https://mega.nz/file/aThlWBSB#7hG3yh6G5hUjX2Dy-1Kqjqwq9gSAREJJeWqyeS1K_m8">Debug Symbols (PDB)</a>
<p>

## <p align="center">Contributing</p>
<p align="center">
If you feel fit to contribute, feel free to create a pull request.
<br>
But please keep your pull request small and understandable and make sure to follow a few rules listed below:
</p>
<dl align="center">
    <dt><u>Keep your code simple and readable</u></dt>
    <dd>- Always use brackets after control structure.
    <br>- If the scope of a control structure is one line, keep the opening bracket on the same line.
    <br>- Blatantly copied pseudocode is not allowed; you should rewrite the pseudocode to some degree so it is understandable and readable.
    </dd>
    <dt><u>Use correct file for declaration/definition</u></dt>
    <dd>- If you define a class or function, it should be placed in the appropriate file based on debug symbols.
    <br>- If multiple classes need access to each other, you should organize them so they can be included as a single header. In some cases, you will need to create your own file, such as <kbd>string_impl.hh</kbd>, where <kbd>string.hh</kbd> is the original definition.
    </dd>
    <dt><u>Use original naming if possible</u></dt>
    <dd>- If you are reimplementing a class or function, please use the original name from the debug symbols. If there are multiple pieces of code that could be inlined function, it is best to declare it as own function and come up with your own name.</dd>
</dl>
<hr>
<p align="center">
These rules are not strongly enforced, but you should still follow them to some degree to avoid wasting the time of the person who will review the pull request. This person is also not responsible for teaching you C/C++ or the basics of decompilation. Furthermore, as the author of the pull request, you should ensure that the code you write works as intended and does not introduce issues in the future. Taking the time to test and validate your code before submission helps maintain code quality and eases the review process.
</p>

## <p align="center">License</p>
<p align="center">
This project won't include a license as it involves working with debug symbols and pseudocode which means parts of the code may be identical or very similar to the original code.<br>
This code is intended solely for educational, documentation, and modding purposes.<br>
We do not condone piracy or the use of this project for commercial activities.<br>
We kindly request that any derivative work remains open-source, with proper given credits.
</p>