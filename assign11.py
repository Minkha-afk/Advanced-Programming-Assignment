from abc import ABC, abstractmethod


class LibraryItem(ABC):

    total_items_created = 0

    def __init__(self, title, year):
        self.title = title
        self.year = year

        LibraryItem.total_items_created += 1
        self.item_id = f"ITEM-{LibraryItem.total_items_created}"

    @abstractmethod
    def display_info(self):
        pass

    def print_common_info(self):    
        print(f"  ID    : {self.item_id}")
        print(f"  Title : {self.title}")
        print(f"  Year  : {self.year}")

    @staticmethod
    def get_total_items_created():
        return LibraryItem.total_items_created


class Book(LibraryItem):

    def __init__(self, title, year, author, pages=0, genre="Unknown"):
        super().__init__(title, year)
        self.author = author
        self.pages = pages
        self.genre = genre

    def display_info(self):
        print("┌─── BOOK ─────────────────────────────")
        self.print_common_info()
        print(f"  Author: {self.author}")
        print(f"  Genre : {self.genre}")
        print(f"  Pages : {self.pages if self.pages > 0 else 'N/A'}")
        print("└──────────────────────────────────────")


class DVD(LibraryItem):

    def __init__(self, title, year, director, duration_minutes, genre, rating="NR"):
        super().__init__(title, year)
        self.director = director
        self.duration_minutes = duration_minutes
        self.genre = genre
        self.rating = rating

    def format_duration(self):
        h = self.duration_minutes // 60
        m = self.duration_minutes % 60
        return f"{h}h {m}m" if h > 0 else f"{m}m"

    def display_info(self):
        print("┌─── DVD ──────────────────────────────")
        self.print_common_info()
        print(f"  Director : {self.director}")
        print(f"  Genre    : {self.genre}")
        print(f"  Duration : {self.format_duration()}")
        print(f"  Rating   : {self.rating}")
        print("└──────────────────────────────────────")


def main():

    catalog = []

    catalog.append(Book(
        "Clean Code",
        2008,
        "Robert C. Martin",
        464,
        "Software Engineering"
    ))

    catalog.append(Book(
        "The Pragmatic Programmer",
        1999,
        "Andrew Hunt",
        genre="Software Engineering"
    ))

    catalog.append(Book(
        "Dune",
        1965,
        "Frank Herbert"
    ))

    catalog.append(DVD(
        "Interstellar",
        2014,
        "Christopher Nolan",
        169,
        "Sci-Fi",
        "PG-13"
    ))

    catalog.append(DVD(
        "Parasite",
        2019,
        "Bong Joon-ho",
        132,
        "Thriller"
    ))

    print("╔══════════════════════════════════════╗")
    print("║           LIBRARY CATALOG            ║")
    print("╚══════════════════════════════════════╝\n")

    for item in catalog:
        item.display_info()
        print()

    print("══════════════════════════════════════")
    print("Total items created :", LibraryItem.get_total_items_created())
    print("══════════════════════════════════════")

    print("\nBooks in catalog:")
    for item in catalog:
        if isinstance(item, Book):
            print(f"{item.title:35} by {item.author}")


if __name__ == "__main__":
    main()