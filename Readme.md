# Dynamic Interval Scheduling

Dynamic Interval Scheduling is a project designed to manage and schedule intervals dynamically. It provides functionalities for booking, deleting, suggesting, and listing intervals, along with saving and loading interval data to/from files.

## Features

- **Book Intervals**: Reserve intervals for specific IDs.
- **Delete Intervals**: Remove reservations based on interval and ID.
- **Suggest Intervals**: Suggest available intervals within a given range.
- **List Bookings**: Display all bookings within a specified range.
- **Save and Load**: Save interval data to a file and load it back.

## Commands

The project supports the following commands:

1. **Book**: `book <start_time> <end_time> <id>`
   - Example: `book 170000 190000 3`
   - Reserves the interval `[start_time, end_time)` for the given ID.

2. **Delete**: `delete <start_time> <end_time> <id>`
   - Example: `delete 170000 190000 3`
   - Removes the reservation for the specified interval and ID.

3. **Suggest**: `suggest <start_time> <end_time>`
   - Example: `suggest 000000 233011`
   - Suggests available intervals within the given range.

4. **Show**: `show <start_time> <end_time>`
   - Example: `show 000000 233011`
   - Lists all bookings within the specified range.

5. **Save**: `save <filename>`
   - Example: `save output.txt`
   - Saves all bookings to the specified file.

6. **Load**: `load <filename>`
   - Example: `load input.txt`
   - Loads bookings from the specified file.
   - You can give complete input in that file (all commands are valid).

## Time Format

The time format used in this project is `HHDDMM`:
- `HH`: Hours (0-based index)
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
   > book 170000 190000 3
   > delete 170000 180000 3
   > suggest 000000 233011
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