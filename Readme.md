# Dynamic Interval Scheduling

Dynamic Interval Scheduling is a project designed to manage and schedule intervals dynamically. It provides functionalities for booking, deleting, suggesting, and listing intervals, along with saving and loading interval data to/from files.

## Features

- **Book Intervals**: Reserve intervals.
- **Delete Intervals**: Remove reservations based on interval .
- **Suggest Intervals**: Suggest available intervals within a given range.
- **List Bookings**: Display all bookings within a specified range.
- **Save and Load**: Save interval data to a file and load it back.

## Commands

The project supports the following commands:

1. **Book**: `book <start_time> <end_time> `
   - Example: `book 17000000 19000000`
   - Reserves the interval `[start_time, end_time)` for the given ID.

2. **Delete**: `delete <start_time> <end_time>`
   - Example: `delete 17000000 19000000`
   - Removes the reservation for the specified interval and ID.

3. **Suggest**: `suggest <start_time> <end_time>`
   - Example: `suggest 00000000 23593011`
   - Suggests available intervals within the given range.

4. **Show**: `show <start_time> <end_time>`
   - Example: `show 00000000 23593011`
   - Lists all bookings within the specified range.

5. **Save**: `save <filename>`
   - Example: `save output.txt`
   - Saves all bookings to the specified file.

6. **Load**: `load <filename>`
   - Example: `load input.txt`
   - Loads bookings from the specified file.
   - You can give complete input in that file (all commands are valid).

## Time Format

The time format used in this project is `HHMMDDMM`:
- `HH`: Hours (0-based index)
- `MM`: Minutes(0-based index)
- `DD`: Days (0-based index)
- `MM`: Months (0-based index)

## File Structure

- `main.cpp`: Contains final implementation.
- `SegmentTree.cpp`: Implementation of Segment Tree for interval management.
- `Treaps.cpp`: Implementation of Treap data structure for dynamic interval suggestions.
- `input.txt`: Example input file for loading interval data.
- `Readme.md`: Documentation for the project.

## How to Run

1. Compile the project using a C++ compiler:
   ```bash
   g++ -o DynamicIntervalScheduling main.cpp 
   ```
2. **Run the Program**:
   Execute the compiled program using the following command:
   ```bash
   ./DynamicIntervalScheduling
   ```
3. **Use the Command-Line Interface**:
   After running the program, a command-line interface will appear. You can input commands one by one to interact with the program. For example:
   ```
   > book 17000000 19000000
   > delete 17000000 18000000
   > suggest 00000000 23593011
   ```
4. **View Output**:
   The program will process each command and display the results in the terminal. If you use the `save` command, the current schedule will be saved to the specified file.
   ```
   > save output.txt
   ```

5. **Load Saved Data**:
   To load previously saved data, use the `load` command:
   ```
   > load output.txt
   ```

6. **Run Tests**:
   To use unit testing, you need to install Google Test (gtest) on your system.
   To run the unit tests provided in `Test.cpp`, use the following commands:
   ```bash
   g++ -std=c++17 -pthread Test.cpp -lgtest -o tests
   ./tests
   ```
   This will compile and execute the tests to verify the functionality of the program.