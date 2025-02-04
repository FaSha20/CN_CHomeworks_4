#ifndef EXCEPTION_H
#define EXCEPTION_H "EXCEPTION_H"

#include <exception>
#include <iostream>

using namespace std;

class ModifySameID: public exception
{
public:        
    ModifySameID()
    {
        cout<< "Can't Modify Same IDs\n";
    }
};

class ShowEmptyTable: public exception
{
public:        
    ShowEmptyTable()
    {
        cout<< "No Topology Created\n";
    }
};

class IDsAlike: public exception
{
public:        
    IDsAlike()
    {
        cout<< "Source and Destination node IDs are alike\n";
    }
};

class BadRequestError: public exception
{
public:        
    BadRequestError()
    {
        cout<< "Bad Request\n";//empty line, query not found cuz wrong order
    }

};

class NotFoundError: public exception
{
public:        
    NotFoundError()
    {
        cout<< "Not Found\n";//query not found cuz wrong propound
    }
};

class PermissionDeniedError: public exception
{
public:        
    PermissionDeniedError()
    {
        cout<< "Permission Denied\n";
    }
};

class EmptyError: public exception
{
public:        
    EmptyError()
    {
        cout<< "Empty\n";
    }
};

class EnoughCredit: public exception
{
public:        
    EnoughCredit()
    {
        cout<< "Not Enough Credit\n";
    }
};

class EnoughRoom: public exception
{
public:        
    EnoughRoom()
    {
        cout<< "Not Enough Room\n";
    }
};

class DiviededInZero: public exception
{
public:        
    DiviededInZero()
    {
        cout<< "Divieded In Zero\n";
    }
};

class NoRatingError: public exception
{
public:        
    NoRatingError()
    {
        cout<< "No Rating\n";
    }
};

class InsufficientRatings : public exception
{
public:
    InsufficientRatings()
    {
        cout<<"Insufficient Ratings\n";
    }
};

#endif
