### [return to home](https://github.com/Sertainty)

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

### sample_delegate
- Sertainty delegate management sample program
- Uses XML to create ID.### sample_id
  - Sets up Services db
  - Creates ID from xml for admin to DB
  - Creates 2nd user ID from XML
  - Creates a subscription to the delegate ID using user 2
  - Creates UXP using Delegate ID
  - Adds data
  - Closes UXP
  - Opens UXP
  - Authenticates as User2
  - On success data is extracted
- using native C++ language interface

### sample_id
- Sertainty sample program;  Uses XML to create ID.
- using C, C++ and C# language interfaces

### sample_kcl
- Sertainty sample program;  Uses KCL to create ID.
- using C, C++ language interfaces

### sample_sm
- Sertainty SmartMessage sample program. Uses XML to create ID. New ID is used as a single sign-on object. SmartMessage created linking ID.
- A UXP SmartMessage requires authentication of the prospective user. Access will only be granted after a successful authentication.  The process of authentication can be tedious if a user accesses multiple UXP objects or the same UXP object multiple times. As a convenience, a Sertainty ID can be used as a single sign-on session, which will automatically authenticate UXP objects / SmartMessages that were created using the ID.
- For authentication, there are two approaches to programmatically seeking authorization. The first approach is to declare a function callback that is called when the system presents challenges.  The callback function is given the list of challenges that it must process and return. The callback function is called until resolution is reached.
- The second uses a looping process to allow the program to handle the challenge list manually. Responses are then given back to the system and the authentication loop continues until a resolution is reached.
- using C,and C++ language interfaces

### sample_sql
- Sertainty sample program. Uses XML to create ID.
- using SQL, C amd native C++ language interface

### sample_sso
- Sertainty single sign-on sample program. Uses XML to create ID.  New ID is used as a single sign-on object.
- using C, C++ language interfaces

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
- Sertainty SmartMessage sample program. Uses XML to create ID. New ID is used as a single sign-on object. SmartMessage created linking ID.
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

### uxp_sql
- Sertainty sample workflow using UXP as a SQL db and native C++ language interface.  Uses XML to create ID.
  - (1) Creates ID from xml
  - (2) Creates UXP from ID
  - (3) Creates SQL tables within UXP object
  - (4) Close / Re-opens UXP
  - (5) Accesses SQL / data
- using C, C++ language interfaces

### webservice.cs
- WebService represents the connection to the Sertainty Data Services Web Service.
  - In order to construct a WebService object, you need:
    - a valid URL to Sertainty Data Services Web Service
     - an encryption key which is has been pre set on the Data Services database using ds::setAccessCode call.
    - A WebService object also holds the Session identifier which is set automatically once a valid session is open.
    - The Session identifier uniquely identifies each client connection made to the Data Services Web Service.
  - Using C#
  
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
- This sample code opens the newly created database and sets the Server, Accesscode and the Trusted Paths.

### 4-delegate_add_users
- This sample code opens the newly created database and adds the Users and User IDs.

### 5-delegate_add_delegates
- This sample code opens the newly created database and adds the Delegates.

### 6-delegate_add_suscribers
- This sample code opens the newly created database and adds the Subscribers.
  
### 7-delegate_info
- This sample code tests all the UXL methods that have to do with a Delegate's Journals, Sytem Flags, Access Codes, and more.
  
### export
- This sample shows how to extract a document from a UXP.

### id_from_xml-V2
- This program shows how you can create a UXP file using a Sertainty Version 2 ID.
- Adds data files into the created UXP Object.

### open_uxp_auth-v2id
- This sample shows how to open and interactively authenticate a UXP Object using known prompt/response pairs and read contents of virtual files protected within a UXP Object.
  
### sample_workflow_delegate
- Demonstrates the features of a UXP Sertainty Services database using UXL scripting.
  - Sertainty Services database is a UXP Object that permits SQL access. The database is used to manage Users and Delegate Identities.
  - A Delegate can be used to allow a User to access a UXP Object without having an identity embedded within the UXP Object.
  - Access is still controlled by the data owner, but is managed via the Sertainty Services database utilized by Delegate Service.
  - To work propertly, the current working Delegate should be the example data folder.
  
### uxp_events
- Demonstrates the features of a UXP recording events at a Sertainty Services.
  - Creates a sample ID that will record events to a Sertainty Services server.
  - Creates the Sertainty Services database and registers the sample ID in order to accept events.
  - Selects event records from the Sertainty Services database.

### webservices_uxp
- Demonstrates the ability to test UXP functionality by calling Sertainty Web Services from UXL.
  - Opens an existing services database.
  - Expects server access code to be 01234567890123456789012345678901.
  - Creates UXP
  - Adds data to the UXP
  - Reads virtual file and writes out to copy1.pdf
  - Closes UXP
  - Re-Opens UXP
  - Extracts file from UXP
  - Closes UXP

### webservice
- Demonstrates the ability to call Sertainty Web Services from UXL.
  - Opens an existing services database.
  - Selects items from the database.
  - Expects server access code to be 01234567890123456789012345678901.




