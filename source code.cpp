#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstring>

using namespace std;



struct Route
{
    string name;
    double distance;
    string weather;
};

struct Seat
{
    string seatNumber;
    string passengerName;
    time_t reservationTime;
};

struct Bus
{
    string busNumber;
    string departureLocation;
    string arrivalLocation;
    string departureTime;
    string arrivalTime;
    int totalSeats;
    vector<Seat> availableSeats;
    vector<Seat> reservedSeats;
    vector<Seat> extraSeats;
    vector<Seat> standbyPassengers;
   
    vector<Route> routes;
    string status;
    string shift;
   
};




void displaySeat(const Seat &seat)
{
    cout << "Seat Number: " << seat.seatNumber << endl;
    cout << "Passenger Name: " << seat.passengerName << endl;
    cout << "Reservation Time: " << ctime(&seat.reservationTime);
    cout << endl;
}
void viewAllPassengers(const vector<Bus> &busList)
{
    cout << "********** Passenger Lists **********" << endl;
    for (const Bus &bus : busList)
    {
        cout << "Bus Number: " << bus.busNumber << endl;
        cout << "Reserved Seats: ";
        if (!bus.reservedSeats.empty())
        {
            for (const Seat &seat : bus.reservedSeats)
            {
                displaySeat(seat);
            }
        }
        else
        {
            cout << "No reserved seats" << endl;
        }
        cout << endl;
    }
    cout << "************************************" << endl;
}

void selectStandbyPassengers(vector<Bus> &busList)
{
    cout << "********** Select Standby Passengers **********" << endl;
    for (Bus &bus : busList)
    {
        cout << "Bus Number: " << bus.busNumber << endl;
        cout << "Available Standby Passengers: " << bus.standbyPassengers.size() << endl;
        cout << "Selecting 5 standby passengers based on reservation time..." << endl;

       
        sort(bus.standbyPassengers.begin(), bus.standbyPassengers.end(),
             [](const Seat &a, const Seat &b)
             { return a.reservationTime < b.reservationTime; });

        int count = 0;
        vector<Seat> selectedStandbyPassengers;
        for (const Seat &seat : bus.standbyPassengers)
        {
            if (count == 5)
                break;
            displaySeat(seat);
            selectedStandbyPassengers.push_back(seat);
            count++;
        }
        cout << endl;

        for (const Seat &seat : selectedStandbyPassengers)
        {
            auto it = find_if(bus.standbyPassengers.begin(), bus.standbyPassengers.end(),
                              [&seat](const Seat &s)
                              { return s.seatNumber == seat.seatNumber; });

            if (it != bus.standbyPassengers.end())
            {
                bus.reservedSeats.push_back(*it);
                bus.standbyPassengers.erase(it);
            }
        }

        if (bus.standbyPassengers.size() > 0 && bus.standbyPassengers.size() < 5)
        {
            cout << "Removing remaining standby passengers..." << endl;
            for (const Seat &seat : bus.standbyPassengers)
            {
                displaySeat(seat);
            }
            cout << endl;

            bus.standbyPassengers.clear();
        }

        bus.status = "Booked";
    }
    cout << "**********************************************" << endl;
}

void initializeBus(Bus &bus, const string &busNumber, const string &departureLocation, const string &arrivalLocation, const string &departureTime, const string &arrivalTime, int totalSeats, const string &shift)
{
    bus.busNumber = busNumber;
    bus.departureLocation = departureLocation;
    bus.arrivalLocation = arrivalLocation;
    bus.departureTime = departureTime;
    bus.arrivalTime = arrivalTime;
    bus.totalSeats = totalSeats;
    bus.availableSeats.clear();
    bus.reservedSeats.clear();
    bus.extraSeats.clear();
    bus.standbyPassengers.clear();
    bus.status = "Scheduled";
    bus.shift = shift;
   

    for (int i = 1; i <= totalSeats; ++i)
    {
        Seat seat;
        seat.seatNumber = "A" + to_string(i);
        seat.passengerName = "";
        seat.reservationTime = 0;
        bus.availableSeats.push_back(seat);
    }

    for (int i = 1; i <= 5; ++i)
    {
        Seat extraSeat;
        extraSeat.seatNumber = "E" + to_string(i);
        extraSeat.passengerName = "";
        extraSeat.reservationTime = 0;
        bus.extraSeats.push_back(extraSeat);
    }
}

void displayBus(const Bus &bus)
{
    cout << "Bus Number: " << bus.busNumber << endl;
    cout << "Departure Location: " << bus.departureLocation << endl;
    cout << "Arrival Location: " << bus.arrivalLocation << endl;
    cout << "Departure Time: " << bus.departureTime << endl;
    cout << "Arrival Time: " << bus.arrivalTime << endl;
    cout << "Total Seats: " << bus.totalSeats << endl;
    cout << "Available Seats: " << bus.availableSeats.size() << endl;
    cout << "Reserved Seats: " << bus.reservedSeats.size() << endl;
    cout << "Extra Seats: " << bus.extraSeats.size() << endl;
    cout << "Standby Passengers: " << bus.standbyPassengers.size() << endl;
    cout << "Status: " << bus.status << endl;
    //cout << "Shift: " << bus.shift << endl;
    cout << "\n\n";
}

void viewAllBuses(const vector<Bus> &busList)
{
    cout << "All Buses:" << endl;
    for (const auto &bus : busList)
    {
        displayBus(bus);
    }
}

void viewAvailableSeats(const vector<Bus> &busList)
{
    cout << "Available Seats for Each Bus:" << endl;
    for (const auto &bus : busList)
    {
        if (bus.status != "Problem")
        {
            cout << "Bus Number: " << bus.busNumber << endl;
            cout << "Available Seats: " << bus.availableSeats.size() << endl;
            cout << endl;
        }
    }
}

void displayReservedSeats(const vector<Bus> &busList)
{
    cout << "********** Reserved Seats **********" << endl;
    for (const Bus &bus : busList)
    {
        cout << "Bus Number: " << bus.busNumber << endl;
        cout << "Reserved Seats: ";
        if (!bus.reservedSeats.empty())
        {
            for (const Seat &seat : bus.reservedSeats)
            {
                displaySeat(seat);
            }
        }
        else
        {
            cout << "No reserved seats" << endl;
        }
        cout << endl;
    }
    cout << "************************************" << endl;
}

void bookSeat(vector<Bus> &busList, const string &busNumber)
{
    for (auto &bus : busList)
    {
        if (bus.busNumber == busNumber)
        {
            if (bus.status == "Problem")
            {
                cout << "Sorry, this bus has an issue and you cannot book a ticket for it." << endl;
                return;
            }
            else if (bus.status == "Booked")
            {
                cout << "Sorry, Bus is fully booked" << endl;
                return;
            }

            int numSeats;
            cout << "Enter the number of seats you want to book (maximum 3 seats): ";
            cin >> numSeats;

            if (numSeats < 1 || numSeats > 3)
            {
                cout << "Invalid number of seats. Please try again." << endl;
                return;
            }

            if (bus.availableSeats.size() >= numSeats)
            {
                vector<Seat> bookedSeats;
                for (int i = 0; i < numSeats; ++i)
                {
                    Seat seat = bus.availableSeats.back();
                    seat.reservationTime = time(0);
                    bus.availableSeats.pop_back();

                    string passengerName;
                    cout << "Enter the name of Passenger " << (i + 1) << ": ";
                    cin >> passengerName;
                    seat.passengerName = passengerName;

                    bookedSeats.push_back(seat);
                }
                bus.reservedSeats.insert(bus.reservedSeats.end(), bookedSeats.begin(), bookedSeats.end());
                cout << "Booking successful! " << numSeats << " seat(s) booked for Bus " << busNumber << "." << endl;
            }
            else if (bus.availableSeats.empty() && bus.extraSeats.size() >= numSeats && numSeats < 5)
            {
                if (bus.availableSeats.size() == 0 && numSeats > 1)
                {
                    cout << "Sorry, there are no available seats on Bus " << busNumber << "." << endl;
                    cout << "Only one standby seat can be allocated." << endl;
                    numSeats = 1;
                }

                vector<Seat> bookedSeats;
                for (int i = 0; i < numSeats; ++i)
                {
                    Seat seat = bus.extraSeats.back();
                    seat.reservationTime = time(0);
                    bus.extraSeats.pop_back();

                    string passengerName;
                    cout << "Enter the name of Passenger " << (i + 1) << ": ";
                    cin >> passengerName;
                    seat.passengerName = passengerName;

                    bookedSeats.push_back(seat);
                }
                bus.standbyPassengers.insert(bus.standbyPassengers.end(), bookedSeats.begin(), bookedSeats.end());
                cout << "Booking successful! " << numSeats << " seat(s) added to standby passengers for Bus " << busNumber << "." << endl;
            }
            else
            {
                cout << "Sorry, there are not enough available seats on Bus " << busNumber << "." << endl;
                return;
            }
            return;
        }
    }

    cout << "Bus with number " << busNumber << " not found." << endl;
}

void viewStandbyPassengers(const vector<Bus> &busList)
{
    cout << "********** Standby Passengers **********" << endl;
    for (const Bus &bus : busList)
    {
        cout << "Bus Number: " << bus.busNumber << endl;
        cout << "Standby Passengers: ";
        if (!bus.standbyPassengers.empty())
        {
            for (const Seat &seat : bus.standbyPassengers)
            {
                displaySeat(seat);
            }
        }
        else
        {
            cout << "No standby passengers" << endl;
        }
        cout << endl;
    }
    cout << "****************************************" << endl;
}

void displayRoutes(const vector<Bus> &busList, const string &busNumber)
{
    for (const Bus &bus : busList)
    {
        if (strcmp(bus.busNumber.c_str(), busNumber.c_str()) == 0)
        {
            cout << "Routes for Bus " << busNumber << ":" << endl;
            for (const Route &route : bus.routes)
            {
                cout << "Route Name: " << route.name << endl;
                cout << "Distance: " << route.distance << " km" << endl;
                cout << "Weather: " << route.weather << endl;
                 cout << "Shift: " << bus.shift << endl;
                
                cout << endl;
            }
            return;
        }
    }
    cout << "Bus with number " << busNumber << " not found." << endl;
}

int main()
{
    vector<Bus> busList;
    Bus bus1;
    initializeBus(bus1, "101", "Karachi", "Lahore", "10:00 AM", "12:00 PM", 0, "Morning");
    busList.push_back(bus1);

    Bus bus2;
    initializeBus(bus2, "102", "Lahore", "Islamabad", "02:00 PM", "05:00 PM", 15, "Afternoon");
    busList.push_back(bus2);

    Bus bus3;
    initializeBus(bus3, "103", "Islamabad", "Rawalpindi", "08:00 AM", "10:30 AM", 15, "Morning");
    busList.push_back(bus3);

    bus1.routes.push_back({"Route 1", 200, "Sunny","morning"});
    bus1.routes.push_back({"Route 2", 300, "Cloudy","evening"});
    bus2.routes.push_back({"Route 3", 250, "Rainy","morning "});
    bus3.routes.push_back({"Route 4", 150, "Sunny","evening"});
    bus3.routes.push_back({"Route 5", 100, "Cloudy","morning"});

    int choice;
    string busNumber, busNumberRoute;
    while (true)
    {
        cout << "********** Daewoo Bus Reservation System **********" << endl;
        cout << "1. View All Buses" << endl;
        cout << "2. View Available Seats for Each Bus" << endl;
        cout << "3. Display Reserved Seats for Each Bus" << endl;
        cout << "4. Book a Seat" << endl;
        cout << "5. View Standby Passengers for Each Bus" << endl;
        cout << "6. Driver Options" << endl;
         cout << "7. Exit" << endl;
        cout << "********************************************" << endl;
        cout << "Enter your choice (1-7): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            viewAllBuses(busList);
            break;
        case 2:
            viewAvailableSeats(busList);
            break;
        case 3:
            displayReservedSeats(busList);
            break;
        case 4:
            cout << "Enter the bus number: ";
            cin >> busNumber;
            bookSeat(busList, busNumber);
            break;
        case 5:
            viewStandbyPassengers(busList);
            break;
        
        case 6:

            cout << "********** Captain Options **********" << endl;
            cout << "1. View All Passengers" << endl;
            cout << "2. Select Standby Passengers" << endl;
            cout << "5. Display Routes and Weather Options for a Bus" << endl; // New option
            cout << "6. Return to Main Menu" << endl;
            cout << "************************************" << endl;
            cout << "Enter your choice (1-6): ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                viewAllPassengers(busList);
                break;
            case 2:
                selectStandbyPassengers(busList);
                break;
           
            case 6:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
            break;
            case 7:
            cout << "Thank you for using the Bus Reservation System. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
