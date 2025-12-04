## Configuration

Create a `config.h` file in the src folder with your WPA2 Enterprise credentials:

```cpp
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#define EAP_IDENTITY "your_hofstra_id_username"
#define EAP_PASSWORD "your_hofstra_password"
#define SSID "eduroam"

#endif
```

**Note:** Make sure to add `config.h` to your `.gitignore` to keep your credentials secure.

Code for UI can be found here: https://git.cs.hofstra.edu/h703168431/final_project_ui