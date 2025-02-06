extends Node2D

const RADIUS = 75

var start_direction = 0
var placeholder_angle = 0.
var cards = []
var best_card = null
var trump = Card.Suit.SPADES

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#$Placeholder.position = Vector2(cos((len(cards)+start_direction) * PI/ 2), sin((len(cards) + start_direction)*PI/2)) * RADIUS
	move_placeholder()#$Placeholder.rotation = (len(cards) + 1 + start_direction)*PI/2 + 3*PI/2
	

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	$Placeholder.position = Vector2(cos($Placeholder.rotation + PI/2), sin($Placeholder.rotation + PI/2)) * RADIUS

const MOVE_SPEED = 0.25
func move_placeholder():
	#var pos_tween = create_tween()
	#pos_tween.tween_property($Placeholder, "position", Vector2(cos(-start_index * PI/ 2), sin(start_index*PI/2)) * RADIUS, MOVE_SPEED)
	var rot_tween = create_tween()
	rot_tween.tween_property($Placeholder, "rotation", (len(cards) + 1 + start_direction)*PI/2 + 3*PI/2, MOVE_SPEED)


func add_child_card(card):
	var global_pos = card.global_position  # Save global position
	var global_rot = card.global_rotation  # Save global rotation
	card.get_parent().remove_child(card)  # Remove from old parent
	add_child(card)  # Add to new parent
	card.global_position = global_pos  # Restore position
	card.global_rotation = global_rot  # Restore rotation
	
	cards.append(card)
	if best_card == null or Card.generate_order(trump).call(cards[best_card], card):
		best_card = len(cards) - 1
	print("-----")
	print(card.rotation_degrees)
	print($Placeholder.rotation_degrees)
	if card.rotation_degrees < 0:
		print("if ", card.rotation_degrees)
		card.rotation_degrees += 360
	print(card.rotation_degrees)
	print(fmod($Placeholder.rotation-0.01, 2 * PI))
	cards.back().tween_position($Placeholder.position, fmod($Placeholder.rotation-0.01, 2 * PI)+0.01 + deg_to_rad(randi()%50 - 25), .5)
	cards.back().z_index = len(cards) - 1
	if len(cards) < 4:
		move_placeholder()
	else:
		get_tree().create_timer(1.).timeout.connect(bring_all_cards)

func bring_all_cards():
	for card in cards:
		card.tween_position(Vector2.ZERO, 0, 1.)
