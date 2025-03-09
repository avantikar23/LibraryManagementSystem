# LibraryManagementSystem
The given **Library Management System (LMS) in C++** follows a structured **Object-Oriented Programming (OOP) approach**, ensuring modularity and scalability. It consists of multiple classes representing different entities:  

- The **`Book`** class stores details like title, author, publisher, ISBN, and availability status. It provides methods for displaying books and updating their status when borrowed or returned.  
- The **`User`** class is an abstract base class with attributes like name, ID, and role. It includes virtual functions that are overridden in derived classes:  
  - **`Student`** (can borrow up to 3 books for 15 days)  
  - **`Faculty`** (can borrow up to 5 books for 30 days)  
  - **`Librarian`** (manages books and users but cannot borrow books)  
- The **`Account`** class tracks borrowed books and calculates fines for overdue books.  
- The **`Library`** class manages collections of books and users, allowing book searching and user display.  

The **main function** provides a **menu-driven interface** that allows users to view books and users, borrow and return books, pay fines, and search books by ISBN. It utilizes objects of the **Library, Student, Faculty, and Account** classes to simulate library operations. File handling is included for **saving and loading books**, but user data persistence is not implemented.  

This system effectively demonstrates OOP principles such as **inheritance (User subclasses), polymorphism (overridden methods), and encapsulation (data hiding in classes)**.
