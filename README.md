# About:

A run-to-completion-scheduler (RTCS) module. This scheduler is nonpreemptive with low overhead. All tasks are given a unique priority value. A RTCS scheduler has the characteristic of always running high priority tasks even at the expense of completely starving lower priority tasks.

This module is comes packaged with UNIT tests. The [Unity Test Framework](https://github.com/ThrowTheSwitch/Unity) is used.

# Adding the module to your project:

Option 1) Clone this repository into your project directory and add only the `src` files to your project.

Option 2) Manually copy and add the `src` files to your project. This option doesn't allow you to run UNIT test on the RTCS files.

# UNIT Test Setup:

1. Install [Ruby](https://rubyinstaller.org/)

2. Ensure you have GCC installed

3. Ensure you have gcc-multilib installed: `sudo apt install gcc-multilib`
    * Otherwise you may see this error: `fatal error: sys/cdefs.h: No such file or directory`

4. Clone the [Unity](https://github.com/ThrowTheSwitch/Unity) repository in the same directory level as your project folder.

# Running the UNIT Tests:

Open terminal in the RTCS directory, run `rake` or `rake all` to build and run the UNIT tests.

# Limitations

Your system must be capable of running 32-bit applications. This was done so the unit tests better emulate 32-bit ARM CPUs.
To work around this, comment out all `-m32` occurances in `target_gcc_32.yml`.


