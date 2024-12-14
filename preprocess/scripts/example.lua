local header = preprocess.create_header("example")
header:add_macro("EXAMPLE_MACRO", "1")
header:add_macro("RANDOM_NUMBER", math.random(1, 100))
header:write()
