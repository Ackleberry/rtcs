# About:

A run-to-completion-scheduler (RTCS) module. This scheduler is nonpreemptive with low overhead. All tasks are given a unique priority value. A RTCS scheduler has the characteristic of always running high priority tasks even at the expense of completely starving lower priority tasks.

This module is comes packaged with UNIT tests. The [Unity Test Framework](https://github.com/ThrowTheSwitch/Unity) is used.

# Adding the module to your project:

Option 1) Clone this repository into your project directory and add only the `src` files to your project.

Option 2) Manually copy and add the `src` files to your project. This option doesn't allow you to run UNIT test on the RTCS files.

# UNIT Test Setup:

1. Install [Ruby](https://rubyinstaller.org/)

2. Install latest Ruby Rake gem:

    `gem install rake`

3. Install GCC

4. Clone the [Unity](https://github.com/ThrowTheSwitch/Unity) repository in the same directory level as your project folder.

# Running the UNIT Tests:

Open terminal in the RTCS directory, run `rake` or `rake all` to build and run the UNIT tests.


