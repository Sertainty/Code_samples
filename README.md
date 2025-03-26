# In Progress...

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

### sample_auto_text

### samples_config
- Sample program interface that demonstrates fetching current machine's location information using the Sertainty SDK. Similarly, machine's network interface information can also be fetched from the machine's configuration. This sample only demonstrates location information for brevity - using C, C++ and C# language interfaces; and uxl scripting
  
## sample_workflow_auto_auth
- Demonstrates a mini workflow, resulting in automatic UXP authentication/ SSO;
  - (1) create ID from XML
  - (2) Create UXP from ID & adds data
  - (3) Closes the UXP
  - (4) Auto authenticates success / fail
  - (5) on success, extracts the data
  - (6) closes UXP 

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

### open_uxp_auth-v2id

### sample_workflow_delegate




