<a name="readme-top"></a>
<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/fredzo/awtris/">
    <img src="docs/awtris-logo.png" alt="Logo" height="80">
  </a>

  <!--h3 align="center">Awtris</h3-->

  <p align="center">
    A Tetris game for <a href="https://awtrixdocs.blueforcer.de/#/en-en/README">Awtrix devices</a>!
    <!--br />
    <a href="https://github.com/fredzo/awtris/docs"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/fredzo/awtris/">View Demo</a>
    ·
    <a href="https://github.com/fredzo/awtris/issues">Report Bug</a>
    ·
    <a href="https://github.com/fredzo/awtris/issues">Request Feature</a-->
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<!--details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details-->



<!-- ABOUT THE PROJECT -->
## About The Project

![Awtris][product-screenshot]

This project is a Tetris game designed to run on an [Ulanzi Desktop Clock](https://www.ulanzi.com/products/ulanzi-pixel-smart-clock-2882) paired with
a [Wiimote](https://en.wikipedia.org/wiki/Wii_Remote) controller for game commands.

It's inspired by the work of [Aaron Liddiment](https://github.com/AaronLiddiment) and [Scott Marley](https://www.youtube.com/watch?v=cqmWfE1DSyM).


<!--p align="right">(<a href="#readme-top">back to top</a>)</p-->



### Built With

This project is built this frameworks/libraries:
* [![PlatformIO][PlatformIO]][PlatformIO-url]
* [![ESP32][ESP32]][ESP32-url]
* [![MelodyPlayer][MelodyPlayer]][MelodyPlayer-url]
* [![ESP32Wiimote][ESP32Wiimote]][ESP32Wiimote-url]
* [![FastLed][FastLed]][FastLed-url]
* [![NeoMatrix][NeoMatrix]][NeoMatrix-url]
* [![AutoConnect][AutoConnect]][AutoConnect-url]

<!--p align="right">(<a href="#readme-top">back to top</a>)</p-->


<!-- USAGE EXAMPLES -->
## Usage

### Single Player Mode

The simplest way to use Awitrix is to run it in single player mode.
For that you need **one** [Ulanzi Desktop Clock](https://www.ulanzi.com/products/ulanzi-pixel-smart-clock-2882) and **one** [Wiimote](https://en.wikipedia.org/wiki/Wii_Remote).

_See <a href="#build-and-install">Build And Install</a> for installation procedure._

Once Awtris device is turned on :
1. Press the "1" and "2" buttons on the Wiimote
2. The LED1 will be on when they have finished connecting  
<img width="30%" src="./docs/remocon_led1_on.png" />

You can now start a new game by pressing "1" or "2" button.

Hold the Wiimote horizontally and use "<", ">" and "v" keys to move Tertrominoes and "1" or "2" buttons to rotate Tetrominoes.

### Settings

#### Brightness

Adjust leds brightness by holding "A" button and pressing "+" or "-".

#### Buzzer volume

Adjust the volume of the buzzer by holding "B" (trigger) button and pressing "+" or "-".

#### Background effect

Change the effect displayed on the board's background by pressing "+" or "-" buttons.

Available backgrounds are :
1. None 
2. Plain
3. Swim lanes
3. Chessboard
4. Stripes
5. Waves
6. Plasma 

### Two Player Mode

For Two Player mode you will need **2** [Ulanzi Desktop Clocks](https://www.ulanzi.com/products/ulanzi-pixel-smart-clock-2882) and **2** [Wiimotes](https://en.wikipedia.org/wiki/Wii_Remote).
While the Wiimote is connected to Awtris module via Bluetooth, the communication between the two Awtris modules will run through the Wifi network.
For that you will need to setup Wifi for each Awtris device with [Autoconnect](https://hieromon.github.io/AutoConnect) :
1. Turn on your Awitris device
2. Connect your Wiimote (needed for the Wifi stack to work properly in shared mode with Bluetooth)
2. Use your phone or laptop to connect to the "Awtrix" Wifi Access Point
3. Enter your Wifi password in the <a href="https://en.wikipedia.org/wiki/Captive_portal">captive portal</a> using the [AutoConnect menu](https://hieromon.github.io/AutoConnect/menu.html). 
4. Wait for a few seconds for the password to be saved in the NVS memory (if password has not been saved upon restart, run the procedure one more time)

<div align="center"><img alt="Overview" width="460" src="docs/ov.png" />&emsp;&emsp;&emsp;<img alt="Captiveportal" width="182" src="docs/ov.gif" /></div>


Once both Awtris devices are on and connected to the same Wifi network, press "Home" button on one of the remotes.

This will prompt for a 2 player game on the second device.

Just press "1" or "2" button on the second device to start the 2 player game.

In 2 player mode, each time a player manages 2, 3 or 4 lines at a time, the second player will receive  1, 2 or 4 lines at the bottom of his board.

The game ends whenever one of the players fills his screen.

<!-- BUILD AND INSTALL -->
## Build And Install

### Build

To build the project, you just have to run **Build** command in PlatformIO.
  ```sh
  Build
  ```

### Installation

Installation is performed with **Upload** or **Upload and Monitor** commands in PlatformIO while the Awtris device is connected via USB.
   ```sh
   Upload and Monitor
   ```

<!-- ROADMAP -->
## Roadmap

- [x] Tetris game port to Awtris device using FastLed NeoMatrix
- [x] Connection to Wiimote as a gamepad
- [x] Tetris Music and sound effects using MelodyPlayer library (modified version to support tempo change an volume change)
- [x] Change buzzer volume and LED brightness using Wiimote
- [x] Change background effect using Wiimote
- [x] Save settings (volume, brightness, background effect, high score) to NVS memory
- [x] Connect the device to Wifi with AutoConnect
- [x] 2 Player mode
- [ ] Documentation
    - [x] English Readme
    - [ ] French Readme 
    - [ ] French Tutorial to create a Tetris game

See the [open issues](https://github.com/fredzo/awtris/issues) for a full list of proposed features (and known issues).

<!--p align="right">(<a href="#readme-top">back to top</a>)</p-->



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<!--p align="right">(<a href="#readme-top">back to top</a>)</p-->



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<!--p align="right">(<a href="#readme-top">back to top</a>)</p-->



<!-- CONTACT -->
## Contact

Your Name - [@your_twitter](https://twitter.com/your_username) - email@example.com

Project Link: [https://github.com/your_username/repo_name](https://github.com/your_username/repo_name)

<!--p align="right">(<a href="#readme-top">back to top</a>)</p-->




<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/fredzo/awtris.svg?style=for-the-badge
[contributors-url]: https://github.com/fredzo/awtris/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/fredzo/awtris.svg?style=for-the-badge
[forks-url]: https://github.com/fredzo/awtris/network/members
[stars-shield]: https://img.shields.io/github/stars/fredzo/awtris.svg?style=for-the-badge
[stars-url]: https://github.com/fredzo/awtris/stargazers
[issues-shield]: https://img.shields.io/github/issues/fredzo/awtris.svg?style=for-the-badge
[issues-url]: https://github.com/fredzo/awtris/issues
[product-screenshot]: docs/awtris.png
[PlatformIO]: https://img.shields.io/badge/platformio.org-000000?style=for-the-badge&logo=platformio&logoColor=orange
[PlatformIO-url]: https://platformio.org/
[ESP32]: https://img.shields.io/badge/espressif.com-000000?style=for-the-badge&logo=espressif&logoColor=red
[ESP32-url]: https://espressif.com/en/products/socs/esp32
[MelodyPlayer]: https://img.shields.io/badge/MelodyPlayer-000000?style=for-the-badge&logo=github&logoColor=cyan
[MelodyPlayer-url]: https://github.com/fabianoriccardi/melody-player
[ESP32Wiimote]: https://img.shields.io/badge/ESP32Wiimote-000000?style=for-the-badge&logo=github&logoColor=cyan
[ESP32Wiimote-url]: https://github.com/hrgraf/ESP32Wiimote
[FastLed]: https://img.shields.io/badge/FastLed-000000?style=for-the-badge&logo=github&logoColor=cyan
[FastLed-url]: https://github.com/FastLED/FastLED
[NeoMatrix]: https://img.shields.io/badge/NeoMatrix-000000?style=for-the-badge&logo=github&logoColor=cyan
[NeoMatrix-url]: https://github.com/marcmerlin/FastLED_NeoMatrix
[AutoConnect]: https://img.shields.io/badge/AutoConnect-000000?style=for-the-badge&logo=github&logoColor=cyan
[AutoConnect-url]: https://github.com/hieromon/AutoConnect
