# Scripting

A Pixelpart effect played with a *PixelpartEffect* or *PixelpartEffect2D* node in your scene can be modified at runtime with a script. This can be used to dynamically make an effect react on whatever is happening in your scene.

## Effect inputs

The following example shows how an effect input can be set with *GDScript*:

~~~
func _ready():
	# Set effect input "Size" to 2.0
	$SampleEffect.set_input_float("Size", 2.0)
~~~

## Triggers

The following example shows how a trigger can be activated with *GDScript*:

~~~
func _ready():
	# Activate the trigger "Event"
	$SampleEffect.activate_trigger("Event")
~~~
