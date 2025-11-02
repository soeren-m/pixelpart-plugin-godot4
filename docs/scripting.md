# Scripting

A Pixelpart effect played with a godot::PixelpartEffect or godot::PixelpartEffect2D node in your scene can be modified at runtime with a script. This can be used to dynamically make an effect react to whatever is happening in your scene.

## Effect inputs

Effect inputs are the primary technique to dynamically modify effect properties. The following example shows how the value of an effect input, which has been defined in the Pixelpart editor, can be changed with *GDScript*:

~~~
func _ready():
	# Set effect input "Size" to 2.0
	$SampleEffect.set_input_float("Size", 2.0)
~~~

Depending on the data type, several *set* methods are available to change the value of effect inputs:

- godot::PixelpartEffect::set_input_bool
- godot::PixelpartEffect::set_input_int
- godot::PixelpartEffect::set_input_float
- godot::PixelpartEffect::set_input_float2
- godot::PixelpartEffect::set_input_float3
- godot::PixelpartEffect::set_input_float4

To retrieve the currently set input value, use the corresponding *get* methods:

- godot::PixelpartEffect::get_input_bool
- godot::PixelpartEffect::get_input_int
- godot::PixelpartEffect::get_input_float
- godot::PixelpartEffect::get_input_float2
- godot::PixelpartEffect::get_input_float3
- godot::PixelpartEffect::get_input_float4

## Triggers

Triggers are used to make effects react to events in your game. The following example shows how a trigger can be activated with *GDScript* using the godot::PixelpartEffect::activate_trigger method:

~~~
func _ready():
	# Activate the trigger "Event"
	$SampleEffect.activate_trigger("Event")
~~~

To check if a trigger has already been activated, use godot::PixelpartEffect::is_trigger_activated.
