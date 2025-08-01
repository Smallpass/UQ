(Week 1)

==============
Java vs Python
==============

Python is an interpreted language, run using the python interpreter. However,
Java is a compiled language, run by first compiling the code into bytecode, then executed.

Hello World:
public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("hello, world");
    }
}

Python checks for type errors while it is runnings (dynamically typed, user input). Java checks 
for error types while it is being compiled (statically typed). 

Printing with:
Java:
if (false) { 
     System.out.println(4 < "5");
    }
    
Python:
    if False:
        print(4<"5")

Java static typing:
    initialiser of value is key. For example:
    int x
    x = 3 is ok
    x = "3" is not ok as it is a string not int

===============================
Java Syntax and Arithmetic 
===============================

All types (int, long, etc) have a number limit. For example:
    The limit of int is -2 147 483 648 to 2 147 483 647. Therefore:
    2 147 483 647 + 1 = -2 147 483 648 as it wraps around. 

Addition: x + y
Subtraction: x - y
Multiplication: x * y
Division: 3/2 returns 1 but 3/2.0 returns 1.5
Remainders: The result has the save sign as the dividend: 5%2 = 1, -5%2 = 1, 5%-2 = -1
Exponential: No operator for exponentiation: Math.pow(2,3) = 2**3 in python
Less than: x<y
Less than or equal to: x<=y
Greater than: x>y
Greater than or equal to:x>=y
Equal: x==y
Not equal: x!=y

And: x&&y
or: x||y
not x: !x
x+=y == x+=y
x-=y == x-=y
Python:
x+=1 = x++
x-=1 = x--

Switch expressions: cover this:

(Week 2)
===============
Strings in java
===============
Strings are ordered mutable sequences of characters (type char).
Strings start index at 0

String dingos = "the quick brown ding";
system.out.println(dingos.length()) = 21
system.out.println(dingos.charAt(0)) = 'T'
system.out.println(dingos.charAt(4)) = 'q'
system.out.println(dingos.substring(0,3)) = 'The'
    note that 3 is not included
dingos.substring(-1) is not possible in java like it is python.
    instead use for example dingos.charAt(dingos.length()-1)
''!=""    "" is used for strings, '' is used for a characters

===============
Java has Arrays
===============

Java arrays fixed length, Chars cannot be added on the end.
They are homogeneous, meaning all chars must have the same type. 
Examples:
int[] numbers = {1,2,3,4,5}
numbers.push(6); does not work as cannot add to array.
numbers[0] = 0; is ok as it just changes a char, {0,2,3,4,5}.
numbers[0] = "number"; does not work as string is not same type as int.

There are two ways to create an arra:
 - Create an empty array of set length: float[] ducks = new float[10];
     - This will create an array length 10 with all characters being the float 0.0f.
 - Set characters of array, java will assume length: long[] journeys = {1, 2, 3};

To get array length: array.length;

===========
Abstraction
===========
Kind of glossed over, procedural abstraction relates to the different methods of completing the same
task. Where some may be more efficient than others.
Does this matter to us: No, not in this course.


================
Data abstraction
================
Abstraction refers to the data viewable. If Math.pow(2,3) works, do we care how it does it? For user sake
not really.

=============
Encapsulation
=============
Information hiding:
We want the user to be able to access the least amount of data possible, that way we can changes
how methods work in future without people noticing. It also makes it more simple for the user.
It also means they cant access things we dont want them to.
public - anyone can use
private - only this class can use
protected - (later) Only this class and subclass can use this.
    - Only code in this package can use this.
Unless you have good reason, members should be private.

An invarient is a property of a class that is always true.
For example: The DOB of a student is never in the future.
Public class member must obey invarients, to ensure this is done, we use constructors.

Constructors must ensure all objects initiall satisfy the invarients. This can be done by throwing an 
error when one is broken. If none is explicitly made, java will imply its own.

Accessors must prevent exposing the internal state (information hiding) to prevent breaking invarients.

Mutators must be careful to preserve invarients when updating the state.

OOP (Object oriented programing) is a way of structuing our code.

Static - static members do not rely on any predefined variables. However, as the name suggests, 
non-statics members do.

IMPORT:
In python, to execute a method of some class, you need to import it. However, with java
you can execute it if you know the full class name. This suggests that all java.lang is 
imported automatically.

==============
Stack and Heap
==============
For any recurring method such as factorial(x) = x * factorial(x-1), the data is stored in a timeline 
for as long as the call is active. i.e. local variables exist as long the call is active. 
This is called the call stack which provides an ordered lifetime.

But what if you want some data to last longer than the method? i.e. if you wanted factorial(x-1) to
last after the factorial(x) method is complete.

One point is that stack overflow occurs when there is no return to a method and a stack is constantly 
added too.

To save this data after a method, we place it on a heap. All objects in java are stored to the heap,
and all local variables are stored to the stack (including parameters).


(Week 3)

============
Polymorphism
============

Translates to many form (greek). 

Take for example the cipher method. If we wanted to use a cipher method for integers also, 
we could either create a new method just for ciphering integers, or we could use polymorphism.

Adhoc polymorphism: Operations that can be applied to parameters of different types.
Subtyping polymorphism: Types that can be substituded with each other.
Parametric polymorphism: (Later)

Adhoc:
Within a class, we make multiple methods of the same name, just requiring different types.
Example:
static String what(int value) {
    return number;
}

static String what(String value) {
        return "text";
}

Adhoc is feasable as a result of the signature of each method. The signature is made up of its
name and input type. 

For example:
public boot fish(int strength, int rodlength){...}
public Fish fish(int strength, int rodlength){...}
These two methods are not allowed in the same class as their signatures are the same, fish, int, int.

Issues with adhoc:
Tedious.
Not easily extensible.
Explicit types must be carried around.

Subtyping polymorphism attempts to fix these issues: 
We can treat all growable crops as the one 'thing'.

Best described with an example:
Interfaces specify all the methods that the user has to implament.

interface Exhaustible{
    void rest();
    boolean isTired();
}

(Student class must have rest and isTired methods, otherwise java
doesnt compile)
class Student implements Exhaustible {
    public void rest() {
        sleep();
    }

    public boolean isTired() {
        return true;
    }
    
    public void study(){...}
}

class Teacher implements Exhaustible(
    public void rest() {
        caffinate()
    }

    public boolean isTired() {
        return isMonday();
    }
)

public void fixThem(Exhaustible exhaustible){
    while(exhaustible.isTired()) {
        exhaustible.rest();
    }
}
fixThem(new Student()); -> sleeps
fixThem(new Teacher()); -> caffinates

However, this means that isMonday() itself cannot be called as it is not
mentioned in the interface.

For example: 
Exhaustible human = new Student();
human.study(); -> compile error

To access .study(), we must use:
Exhaustible human = new Student();
if (human instanceof Student stu) {
    stu.Study();
}

What this says is since human is of type student, create an new variable stu, assign it to student and
call study method. It can be seen that if the human was a teacher, the if statement would never be true
and the code would not execute.

New types: to change the type of a variable, we can use (new type) which is called a typecast 
or just cast.
Example:
float x = 7.1;
(int) x returns 7 (does not round, just chops off)

A class that implements an interface is said to be an implementation of the interface. E.G.

Student is an implementation of Exhaustible. This establishes an is-an relationship.
Student is-a Exhaustible.
In the same way, an instance of a class, if implemented in an interface, is called an instance
of the interface.

===========
Inheritance
===========
Exactly like inheriting from parent. 

In java, for a child/sub class to inherit from some parent/super 
class "People", we can say;
class Australians extends People{}

When creating a class, if no extend is explicitly stated, it extends object.
E.g:
class Student{} == class Student extends object {}

The object class has 11 methods, the ones of interest are:
Strinig toString(): get a string to represent the object (basically just printing).
boolean equals(Object): is this object equal to another.
int hashCode(): get a number representing the object.

We can overide these methods if they are not useful to use.
For example,
Triangle triangle = new Triangle(3,4,5);
system.out.println(triangle);
will return a random string of numbers(its storage/hash code)
We can use @Override to indicate that the method has been re-written
in order to make it more useful.
@Override is not required but it can be useful as it identified some errors.

Can and cant do's of inheritance:
Can:
Add new methods or methods (different names)
Overload existing methods (different signature)
Override (redefine) existing methods (same signature)

Can't:
Change the type or parameters of existing methods.
Change the type of member variables.
Tighten access control of any members.

Extend vs implements:
Best explained through example:
class Child extends Parent{}
Cheld gets the public and protected methods and variables.
It also gets the method bodies/implementation (how it does it).

class Child implements Parent{}
Child gets public and protected methods and variable.
Note: It does not get the implementation of the methods.

We can have both implements and extends at the same time.
For example:
public class Sheep extends Mammal{
    Implements Growable, Clonable;
}
This will mean Sheep inherits all code and members from mammal class,
and all methods the Growable and Clonable say they should have.

Warning:
If a class does no have a constructor, a default constructor is created.
This calls the default constructor of the parent class. Constructors
are not inherited.

Casting in inheritance
Casting up the hierarchy (upcast, Bird to Animal) can be implicity. However,
casting down (downcast) cant be.

===============
Handling Errors
===============

Total function = Every input in the domain maps to a value in the range.

Partial function = Some inputs in the domain do not map to a value in the range.
Example: Some functions may only take imputs for values greater than 0, thus it is partial.

This is the same for methods, total and partial. For example,
a triangle area calculator could not use a negative value.

If the user uses our code wrong, we throw an error.

For example, we could create an if statement for the case where the method is violated, 
then we could return nothing (silent failure), a boolean or an error code.

A Sentinel return means that we want a result out even if the method is violated.
This usually means the special value was not meant to be taken literally within the method,
thus we just have a seperate if statement.

Robust program: A program that behaves reasonably even in the presence of errors.

==========
Exceptions
==========
Note: Ignoring exceptions to your method will result in many bugs down the track.

Unwinding: Once an exception has been thrown, it will unwind the stack until caught.
Returns are skipped in favour of unwinding.

For example, if method 2 calls method 1 etc:

method1

method2(method1)

method3(method2)

method4(method3)
returns an arithmetic error, the code will go back up the stack (unwinding it)
until it finds out which method conducted an arithmetic oreration. If no such\
method is found, the program will continue to unwind until it crashes.

To catch an exception, we could implement a catch call in the arithmetic method.
Example:

method2(method1)
    catch (ArithmeticException e){
        return -1;
    }

Alternatively, the exception can be manually triggered by throwing it given a condition.
Example:

public static int fib(int n) {
    if (n < 0) {
        throw new ArithmeticException("fib got n < 0: " +n);
    }
}

Unchecked vs Checked Exceptions:
Unchecked: 
    Can occur at any point during execution.
    May or may not be handled.

Checked:
    Compiler checks they are handled.
    Must be declared if a method can throw it.

(Week 4)

Assignment (x=y)
Equality (x==y)

Checking equality could be asking for if they have the same 'value', or 
could refer to whether they are the exact same object in memory.
Equality (==) refers to if they are the exact same object in memory.

This can also be an issue when dealing with particular values, for example small numbers;
A double can be quite inaccuracte:
double f = 2;
double g = Math.sqrt(Math.sqrt(f));
double h = g*g*g*g;
System.out.print(h==f); -> False
System.out.println(Math.abs(h-f)); -> 4.44*10^(-16)
i.e. they should be equal, but the floats are a little inaccurate.

Object Equality: Objects will be equivalent (a==b is true) when they have the exact
same space in memory.
To check if two values are 'equivalent' but have different places in memory, we need to use
a method.

Mutability: If all access to an objects;s state is via methods,
then we control how the state changes.

e.g Strings are imutable, while arrays and lists are mutable. We try to minimize 
mutability.

Catching a parent exception class will catch any of its children.

Make sure to try to catch the child exception first. For example, EOFException is a 
child class of IOException, thus if:

catch (IOException e) {
    //
}
catch (EOFException e) {
    //
}

will always catch the parent first without even checking for the more specific and useful child 
case.

Making own exceptions:
We can do this by creating an exception class that extends the parent class Exception:
(Idealy done this way)

public class OopsADaisy extends Exception{
    public OopsADaisy() {
        super();
    }

    public OopsADaisy(String message) {
        super(message);
    }

    public OopsASaisy(Exception cause) {
        super(cause);
    }
}

But it is often better to use the common exceptions, some include:

Project Organising:

When the number of identifiers increases, the code from multiple libraries
could clash, i.e, could have same class name.

Packages solve this: Two classes can have the same name as long as they are in different packages.
(Similar to folders on a computer)
You declare which package the class belongs to by putting it at the top of the file.
package 'name';

This finally explains all three types of variables:
public - any code can use this
private - only this class can use this
protected - only this package (and subclasses) can use this

Leaving off an access specifier, i.e int c = 24, it automatically becomes
a protected variable.

===========
Collections
===========

Arrays are ordered fixed-length mutable sequences of homogeneous items.
Not great for everything as we dont always know the intended size when creating it.
They do not automatically close gaps.

For example, we could have a factorial calculator, which outputs each of its values in an array,
however, this may cause issues as we dont know how big the array should be.
A solution to this (although we have been told not to do it) is to call a method that 
combines the array with a new array with needed extra values.

Built-in collections:
Stack, lists, set, map
java.util.* (neet to implament)

Stacks:
Last in first out (LIFO)
empty() Is this stack empty?
peek() Return object at top of stack.
pop() Remove the object at the top of the stack and return it.
push(obj) Put obj on the top of the stack.

If trying to pop off an empty stack, we get an exception.
import java.io.*;
import java.util.*;

public class FileExample {

    public static void main(String[] args) {
        // File path
        String filePath = "data.txt";

        // Writing to a file
        writeToFile(filePath);

        // Reading from a file
        List<String> lines = readFromFile(filePath);
        System.out.println("Contents of the file:");
        for (String line : lines) {
            System.out.println(line);
        }

        // Manipulating lists
        List<Integer> numbers = new ArrayList<>();
        numbers.add(1);
        numbers.add(2);
        numbers.add(3);
        System.out.println("Sum of numbers: " + sum(numbers));

        // Manipulating maps
        Map<String, Integer> map = new HashMap<>();
        map.put("One", 1);
        map.put("Two", 2);
        map.put("Three", 3);
        System.out.println("Value for key 'Two': " + map.get("Two"));

        // String manipulation
        String str = "Hello, World!";
        System.out.println("Length of the string: " + str.length());

        // Integer manipulation
        int num = 42;
        System.out.println("Square of " + num + ": " + square(num));
    }

    // Function to write to a file
    public static void writeToFile(String filePath) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filePath))) {
            writer.write("Hello, world!\n");
            writer.write("This is a test file.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Function to read from a file
    public static List<String> readFromFile(String filePath) {
        List<String> lines = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return lines;
    }

    // Function to calculate the sum of a list of integers
    public static int sum(List<Integer> numbers) {
        int sum = 0;
        for (int num : numbers) {
            sum += num;
        }
        return sum;
    }

    // Function to square an integer
    public static int square(int num) {
        return num * num;
    }
}
package exceptioncustom;

public class CustomExceptionGenerate {

    /**
     * Generates a custom exception based on the given number.
     * If number is less than 30, a NumberTooSmallException must be thrown with
     * the message "The number was too small";
     * If the number is exactly 30, a NumberTooSmallException must be thrown with
     * no message.
     * If number is more than 50, a NumberTooBigException must be thrown with
     * the message "The number was too big";
     * If the number is exactly 50, a NumberTooBigException must be thrown with
     * no message.
     * If the number is more than 30, and less than 50, nothing happens.
     * @param number the number value to be checked
     */
    public static void generateCustomException(int number) throws
            NumberTooBigException, NumberTooSmallException {
        if (number < 30) {
            throw new NumberTooSmallException("The number was too small");
        } else if (number == 30) {
            throw new NumberTooSmallException();
        } else if (number > 50) {
            throw new NumberTooBigException("The number was too big");
        } else if (number == 50) {
            throw new NumberTooBigException();
        }
    }
}

public class LaddergramValidator {

    public static boolean isLaddergram(List<String> sequence) {
        if (sequence == null || sequence.isEmpty()) {
            return false;
        }

        int wordLength = sequence.get(0).length(); 

        for (int i = 0; i < sequence.size() - 1; i++) {
            String currentWord = sequence.get(i);
            String nextWord = sequence.get(i + 1);
            if (currentWord.length() != wordLength || nextWord.length() != wordLength) {
                return false;
            }
            int differenceCount = 0;
            for (int j = 0; j < wordLength; j++) {
                if (currentWord.charAt(j) != nextWord.charAt(j)) {
                    differenceCount++;
                }
                if (differenceCount > 1) {
                    return false;
                }
            }
            if (differenceCount != 1) {
                return false;
            }
        }
        return true;
    }
}

public class Collatz {

    public static int steps(int low, int high) {
        int maxCycleLength = 0;
        for (int i = low; i <= high; i++) {
            int cycleLength = calculateCycleLength(i);
            if (cycleLength > maxCycleLength) {
                maxCycleLength = cycleLength;
            }
        }
        return maxCycleLength;
    }

    private static int calculateCycleLength(int n) {
        int cycleLength = 1;
        while (n != 1) {
            if (n % 2 == 0) {
                n /= 2;
            } else {
                n = 3 * n + 1;
            }
            cycleLength++;
        }
        return cycleLength;
    }
}

public class FragmentSolver {
    public static Set<FileFragment> solve(List<FileFragment> fragments) {
        int totalBits = fragments.stream().mapToInt(FileFragment::getLength).sum();
        int originalSize = totalBits * 2 / fragments.size();

        Set<FileFragment> possibleOriginals = new HashSet<>();

        for (int i = 0; i < fragments.size(); i++) {
            for (int j = i + 1; j < fragments.size(); j++) {
                if (fragments.get(i).getLength() + fragments.get(j).getLength() == originalSize) {
                    FileFragment possibleFile = fragments.get(i).concat(fragments.get(j));
                    possibleOriginals.add(possibleFile);
                }
            }
        }
        Set<FileFragment> validOriginals = new HashSet<>();
        for (FileFragment original : possibleOriginals) {
            boolean isValid = true;
            for (FileFragment fragment : fragments) {
                boolean found = false;
                for (FileFragment other : fragments) {
                    if (original.equals(fragment.concat(other)) || original.equals(other.concat(fragment))) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    isValid = false;
                    break;
                }
            }
            if (isValid) {
                validOriginals.add(original);
            }
        }

        if (validOriginals.isEmpty()) {
            return possibleOriginals;
        } else {
            return validOriginals;
        }
    }
}
