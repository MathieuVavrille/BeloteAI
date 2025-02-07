extends Node
class_name ChildExchange

# Change the child to the new parent, and keeps the right position
static func exchange(child, new_parent):
	var global_pos = child.global_position  # Save global position
	var global_rot = child.global_rotation  # Save global rotation
	child.get_parent().remove_child(child)  # Remove from old parent
	new_parent.add_child(child)  # Add to new parent
	child.global_position = global_pos  # Restore position
	child.global_rotation = global_rot  # Restore rotation
