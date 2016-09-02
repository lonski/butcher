#!/bin/bash

flatc -b actors.fbs actors_data.json
flatc -b effects.fbs effects_data.json
flatc -b levels.fbs levels_data.json
flatc -b recipes.fbs recipes_data.json
