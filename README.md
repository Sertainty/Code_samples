# Code Samples

The Sertainty code samples are organized by languagge as well as function. In some cases the functionally focused samples are included in the language folder and supporting files folders.

# 

# Code Sample Descriptions

## for C, C++, C#, UXL script

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
given back to the system and the authentication loop continues until a resolution is reached - using C, C++ and C# language interfaces; and uxl scripting

### open_uxp_auth
- Demonstrates how to open and interactively authenticate a UXP file using known challenge/response pairs and read contents of virtual files protected within a UXP file - using C, C++ and C# language interfaces; and uxl scripting

### sample.cpp
- Sertainty sample creates uxp object from kcl file
- using C++
  - ### sample.kcl
    - See sample.cpp
    - Sample creates identity from ; KCL file

### sample_auto_text

### samples_config
- Sample program interface that demonstrates fetching current machine's location information using the Sertainty SDK. Similarly, machine's network interface information can also be fetched from the machine's configuration. This sample only demonstrates location information for brevity - using C, C++ and C# language interfaces; and uxl scripting
  
### sample_workflow_auto_auth
- Demonstrates a mini workflow, resulting in automatic UXP authentication/ SSO;
  - (1) create ID from XML
  - (2) Create UXP from ID & adds data
  - (3) Closes the UXP
  - (4) Auto authenticates success / fail
  - (5) on success, extracts the data
  - (6) closes UXP
- using C, C++ and C# language interfaces

### sample_workflow_create_id_uxp_open
- Demonstrates a mini workflow, resulting in UXP authentication and data extraction
  - (1) create ID from XML
  - (2) Create UXP from ID & adds data
  - (3) Closes the UXP
  - (4) Authenticates success / fail
  - (5) on success, extracts the data
  - (6) closes UXP
- using C, C++ and C# language interfaces

### sample_workflow_kcl
- Sample program using native C++ language interface.  Uses KCL to create ID
  - (1) create ID from XML
  - (2) Create UXP from ID & adds data
  - (3) Closes the UXP
  - (4) Authenticates success / fail
  - (5) on success, extracts the data
  - (6) closes UXP
- using C,and C++ language interfaces

### sample_workflow_sm
- Sertainty SmartMessage sample program using native C++ language interface. Uses XML to create ID. New ID is used as a single sign-on object. SmartMessage created linking ID.
- A UXP SmartMessage requires authentication of the prospective user. Access will only be granted after a successful authentication.  The process of authentication can be tedious if a user accesses multiple UXP objects or the same UXP object multiple times. As a convenience, a Sertainty ID can be used as a single sign-on session, which will automatically authenticate UXP objects / SmartMessages that were created using the ID.
- For authentication, there are two approaches to programmatically seeking authorization. The first approach is to declare a function callback that is called when the system presents challenges.  The callback function is given the list of challenges that it must process and return. The callback function is called until resolution is reached.
- The second uses a looping process to allow the program to handle the challenge list manually. Responses are then given back to the system and the authentication loop continues until a resolution is reached.
- using C,and C++ language interfaces

### sample_workflow_sql
- Sertainty sample workflow using UXP as a SQL db and native C++ language interface.  Uses XML to create ID.
  - (1) Creates ID from xml
  - (2) Creates UXP from ID
  - (3) Creates SQL tables within UXP object
  - (4) Close / Re-opens UXP
  - (5) Accesses SQL / data
- using C, C++, and SQL language interfaces, and uxl scripting
 
### uxp_from_id
- This sample demonstrates how to create a UXP Object using a UXP Identity and add data files into the created UXP Object.
- using C, C++ and C# language interfaces; and uxl scripting

## for UXL script only

### 1-Delegate_create_mySql
  - This sample code creates a new mySql Database - using uxl scripting
  - Application expects to find the necessary source files in the current working folder.   
   
### 1-delegate_create_uxpSql
- This sample code creates a new uxpSql Database - using uxl scripting
- Application expects to find the necessary source files in the current working folder.

### 2-delegate_open_session
- This sample allows you to open an existing DS Database and authenticate into it to create a DS Session - using uxl scripting

### 3-delegate_set-server

### 4-delegate_add_users

### 5-delegate_add_delegates

### 6-delegate_add_suscribers

### 7-delegate_info

### export

### id_from_xml-V2
- This program shows how you can create a UXP file using a Sertainty Version 2 ID.
- Adds data files into the created UXP Object.

### open_uxp_auth-v2id

### sample_workflow_delegate

### uxp_events

### webservices_uxp

### webservice




