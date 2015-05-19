cmake_minimum_required(VERSION 3.2)
project(Backgrounds)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    371f3e6eb74280fda8a5452a4eb7abdc.jpg
    4385077-8688400562-berli.jpg
    442782.png
    97461c7ea757f789d99235177b372395.png
    _origin_Pasaules-gali-kurus-2.jpg
    Action-movie-Background1.jpg
    angry_birds___city_background_by_munir99-d4vqlcu.png
    att00001.jpg
    background-development1.jpg
    City_in_Ruins_by_lolgast.png.jpg
    detail_1111_aff_fire.jpg
    Ex1Image.jpg
    j.jpg
    MenuBack.jpg
    MenuBack2.jpg
    metro-last-light-city-screenshot.jpg
    Nuclear_Blast_by_RainerKalwitz.jpg
    premade_background_war_apocalipsis_ii_by_ladiahidoi-d5rgtni.jpg
    premade_background_war_apocalipsis_iv_by_ladiahidoi-d5rz854.jpg
    rimfire-featured.jpg
    "Street Background_mockup2.png")

add_executable(Backgrounds ${SOURCE_FILES})