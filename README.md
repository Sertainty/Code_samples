# In Progress...

The Sertainty code samples are organized by languagge as well as function. In some cases the functionally focused samples are included in the language folder and supporting files folders.

# 

# Code Sample Descriptions

## for C, C++, C#

### Bytearrays
- Demonstrates the use of uxl ByteArray which is a Sertainty Data Structure that is used to handle binary and ascii data.
- The sample demonstrates writing data to and reading data from the uxl ByteArray as well as loading contents of a file read from the file system into the uxl ByteArray - using C, C++ and C# language interfaces and UXL scripting language

### Helloworld
- Demonstrates the initialize library call necessary before any UXP ID or UXP functions can be called - using C, C++ and C# language interfaces; and uxl scripting

### id_from _xml
- This sample demonstrates constructing a UXP Identity file (*.iic) from an ID Definition XML Source file (*.xml). The generated UXP Identity file (*.iic) can be used to generate a UXP Object.

### id_session
- This sample demonstrates how to authenticate into an ID session and use it to auto-authenticate, also known as, Single Sign-On, into a UXP Object.
- A UXP Object requires authentication of the prospective user (process or person). ßAccess will only be granted after a successful authentication.
- Having to individually authenticate into multiple UXP Objects is time-consuming. Also time-consuming is authenticating into a single UXP Object multiple times. As a convenience, a UXP Identity can be used for a Single Sign- On session. A Single Sign-On session allows automatic authentication into UXP Objects that were created using that same UXP Identity.
- For authentication, there are two approaches to programmatically seeking authorization. The first approach is to declare a function callback that is called when the system presents challenges. The callback function is given the list of challenges that it must process and return. The callback function is called until resolution is reached.
- The second uses a looping process to allow the program to handle the challenge list manually. Responses are then
given back to the system and the authentication loop continues until a resolution is reached.

## 

## for UXL script

### 1-Delegate_create_db
  - c, c++,c#, uxl
    - This sample demonstrates how to create a new Data Services Database using an existing ID that will be designated as the DB owner ID. The DataServices db stores and manages Delegate IDs, Users and Delegate subscribers.
