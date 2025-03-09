#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

// Book Class
class Book {
private:
    string title, author, publisher, ISBN;
    int year;
    bool isAvailable;

public:
    Book(string t, string a, string p, string isbn, int y)
        : title(t), author(a), publisher(p), ISBN(isbn), year(y), isAvailable(true) {}

    void displayBook() const {
        cout << "Title: " << title << ", Author: " << author
             << ", Publisher: " << publisher << ", ISBN: " << ISBN
             << ", Year: " << year << ", Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
    }

    string getISBN() const { return ISBN; }
    bool available() const { return isAvailable; }
    void borrowBook() { isAvailable = false; }
    void returnBook() { isAvailable = true; }
};

// User Base Class
class User {
protected:
    string name;
    int id;
    string role;

public:
    User(string n, int i, string r) : name(n), id(i), role(r) {}

    virtual void displayUser() const {
        cout << "ID: " << id << ", Name: " << name << ", Role: " << role << endl;
    }

    virtual bool canBorrowMore(int borrowedBooks) const = 0;
    virtual int getMaxBorrowDays() const = 0;
};

// Student Class
class Student : public User {
public:
    Student(string n, int i) : User(n, i, "Student") {}

    bool canBorrowMore(int borrowedBooks) const override {
        return borrowedBooks < 3;
    }

    int getMaxBorrowDays() const override {
        return 15;
    }
};

// Faculty Class
class Faculty : public User {
public:
    Faculty(string n, int i) : User(n, i, "Faculty") {}

    bool canBorrowMore(int borrowedBooks) const override {
        return borrowedBooks < 5;
    }

    int getMaxBorrowDays() const override {
        return 30;
    }
};

// Librarian Class
class Librarian : public User {
public:
    Librarian(string n, int i) : User(n, i, "Librarian") {}

    bool canBorrowMore(int) const override {
        return false;
    }

    int getMaxBorrowDays() const override {
        return 0;
    }
};

// Account Class
class Account {
private:
    vector<pair<Book, time_t>> borrowedBooks;
    int fineAmount;

public:
    Account() : fineAmount(0) {}

    void borrowBook(Book &book, int maxDays) {
        time_t now = time(0);
        borrowedBooks.push_back({book, now + (maxDays * 86400)});
        book.borrowBook();
    }

    void returnBook(Book &book) {
        time_t now = time(0);
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (it->first.getISBN() == book.getISBN()) {
                int overdueDays = (now - it->second) / 86400;
                if (overdueDays > 0) {
                    fineAmount += overdueDays * 10;
                }
                borrowedBooks.erase(it);
                book.returnBook();
                break;
            }
        }
    }

    int getFineAmount() const {
        return fineAmount;
    }

    void payFine() {
        fineAmount = 0;
    }
};

// Library Class
class Library {
private:
    vector<Book> books;
    vector<User*> users;

public:
    void addBook(Book book) {
        books.push_back(book);
    }

    void displayBooks() const {
        for (const auto &book : books) {
            book.displayBook();
        }
    }

    void addUser(User *user) {
        users.push_back(user);
    }

    void displayUsers() const {
        for (const auto &user : users) {
            user->displayUser();
        }
    }

    Book* searchBookByISBN(string isbn) {
        for (auto &book : books) {
            if (book.getISBN() == isbn) {
                return &book;
            }
        }
        return nullptr;
    }
};

// File Handling Functions
void saveBooksToFile(const vector<Book> &books) {
    ofstream file("books.txt");
    for (const auto &book : books) {
        file << book.getISBN() << endl;
    }
    file.close();
}

void loadBooksFromFile(vector<Book> &books) {
    ifstream file("books.txt");
    string isbn;
    while (file >> isbn) {
        books.push_back(Book("Title", "Author", "Publisher", isbn, 2020));
    }
    file.close();
}

// Main Function
int main() {
    Library lib;

    // Adding books
    lib.addBook(Book("Book1", "Author1", "Publisher1", "ISBN1", 2000));
    lib.addBook(Book("Book2", "Author2", "Publisher2", "ISBN2", 2005));
    lib.addBook(Book("Book3", "Author3", "Publisher3", "ISBN3", 2010));
    lib.addBook(Book("Book4", "Author4", "Publisher4", "ISBN4", 2015));
    lib.addBook(Book("Book5", "Author5", "Publisher5", "ISBN5", 2020));

    // Adding users
    Student s1("Alice", 1);
    Faculty f1("Dr. Bob", 2);
    Librarian l1("Charlie", 3);

    lib.addUser(new Student("Alice", 1));
    lib.addUser(new Faculty("Dr. Bob", 2));
    lib.addUser(new Librarian("Charlie", 3));

    Account studentAccount, facultyAccount;

    int choice;
    do {
        cout << "\nLibrary Menu:\n1. View Books\n2. View Users\n3. Borrow Book\n4. Return Book\n5. Pay Fine\n6. Search Book\n7. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                lib.displayBooks();
                break;
            case 2:
                lib.displayUsers();
                break;
            case 3: {
                string isbn;
                cout << "Enter ISBN of book to borrow: ";
                cin >> isbn;
                Book* book = lib.searchBookByISBN(isbn);
                if (book && book->available()) {
                    studentAccount.borrowBook(*book, s1.getMaxBorrowDays());
                    cout << "Book borrowed successfully!\n";
                } else {
                    cout << "Book not available!\n";
                }
                break;
            }
            case 4: {
                string isbn;
                cout << "Enter ISBN of book to return: ";
                cin >> isbn;
                Book* book = lib.searchBookByISBN(isbn);
                if (book) {
                    studentAccount.returnBook(*book);
                    cout << "Book returned successfully!\n";
                } else {
                    cout << "Book not found!\n";
                }
                break;
            }
            case 5:
                cout << "Fine paid. Account cleared!\n";
                studentAccount.payFine();
                facultyAccount.payFine();
                break;
            case 6: {
                string isbn;
                cout << "Enter ISBN to search: ";
                cin >> isbn;
                Book* book = lib.searchBookByISBN(isbn);
                if (book) book->displayBook();
                else cout << "Book not found!\n";
                break;
            }
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 7);

    return 0;
}
