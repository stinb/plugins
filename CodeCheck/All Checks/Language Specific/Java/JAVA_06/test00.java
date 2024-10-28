import java.util.AbstractMap.SimpleEntry;  // UndCC_Valid
import java.util.HashMap;                  // UndCC_Valid

import java.util.regex.Matcher;            // UndCC_Valid
import java.util.regex.Pattern;            // UndCC_Valid

import recipes.cookies.ChocolateChip;      // UndCC_Valid
import recipes.Smoothies.StrawberryBanana; // UndCC_Violation - known package

import Rocks.Obsidian;                     // UndCC_Violation - known package

import trees.Deciduous.Birch;              // UndCC_Valid - unknown package
import Trees.Birch;                        // UndCC_Valid - unknown package
