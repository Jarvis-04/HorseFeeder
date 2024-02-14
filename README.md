This is a work in progress

This project is designed to aid my mother in creating horse feeds for the horses she looks after. Each horse requires a different blend of feed and it is quite tedious for her to make them each day. 
The general layout is a bucket on a rail that will collect the correct weight of each feed a horse needs. 

The startup and clock setup part of the project was created using STM's CubeMX program. But each component driver is custom-built by me, using only datasheets and other people's implementations as reference. This was done to teach me more about driver development and because I like to have control of all the parts. Premade files are in the Core and Drivers folders, code by me is in the inc and src folders.

Future Goals:  
-Continue to build out core functionality  
-Design a simple user interface to allow the user to customize each horse profile and control the general movement of the system  
-Add the ability for more than just dry feed to be dispensed. Including pure liquids  
