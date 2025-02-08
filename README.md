# Vending Machine Program in C++

A C++ program that simulates a vending machine with user and admin modes. This project demonstrates SQLite database integration (a popular relational database) and simulation logic, while also serving as a practice project for Object-Oriented Programming (OOP) concepts.

## Features

### User Mode

-   Item Selection: Users can select items using unique selection codes.
-   Payment: Pay using denominations (100 THB, 20 THB, 10 THB, 5 THB, 1 THB).
-   Stock Management: Automatically reduces stock by 1 after each purchase.
-   Payment Handling:
    -   Insufficient payment prompts users for additional money.
    -   Returns change upon successful payment.
    -   Out of Stock: Displays "OUT OF STOCK" if an item is unavailable.
-   Stop Operation Conditions:
    -   50% or more product categories are out of stock.
    -   Change box is empty.
    -   Collection box is full.

### Admin Mode:

-   Initial Setup: Set initial stock levels for items.
-   Re-stocking: Refill item stocks as needed.
-   Cash Management:
    -   Check balances in the change box and collection box.
    -   Collect money from the collection box and refill the change box.

## Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/SawZiDunn/vending-machine.git
    ```

2. Go to Project Directory:

    ```bash
    $ cd vending-machine
    ```

3. Install SQLite3 (if not already installed).

## How to Run

1. Compile the program:

    ```bash
    g++ main.cpp -o vending_machine -lsqlite3
    ```

2. Run the program:

    ```bash
    ./vending_machine
    ```

3. Follow the prompts:

    - Choose between User Mode or Admin Mode.

    - Perform actions as prompted by the program.

## Code Structure

-   main.cpp : Allows users to choose between User and Admin Modes initializes the database if not present.
-   db_setup.h : Functions for setting up and populating the database.
-   user.h : Implements user-specific actions
-   admin.h : Implements admin-specific actions
-   db_controller.h : Contains functions that directly interact with the SQLite database
-   utils.h : Helper functions
-   moneyInstance.h : Defines a structure to represent money instances for denomination handling.
