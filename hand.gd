extends Node2D

@export var small_radius = 100
@export var big_radius = 200
@export var natural_order = true

const CARD_SCENE = preload("res://card.tscn")
var min_angle = PI / 5
var max_angle = PI - min_angle
var angle_between_cards = PI / 12

var cards = []
var cards_goal_angles = []
var ANGLE_SPEED = PI / 2


func _input(event):
	if event is InputEventMouseButton and event.pressed:
		for i in range(len(cards)-1, -1, -1):
			if cards[i].is_hovered:
				print(cards[i].suit, " ", cards[i].rank)
				return


func add_child_card(card):
	var global_pos = card.global_position  # Save global position
	var global_rot = card.global_rotation  # Save global rotation
	
	card.get_parent().remove_child(card)  # Remove from old parent
	add_child(card)  # Add to new parent
	card.mouse_movement.connect(on_card_mouse_movement)
	card.global_position = global_pos  # Restore position
	card.global_rotation = global_rot  # Restore rotation
	add_card(card)


func add_card(card):
	cards.append(card)
	cards.sort_custom(Card.generate_order(null))
	position_cards()



func create_card(suit, rank):
	var card = CARD_SCENE.instantiate()
	card.suit = suit
	card.rank = rank
	card.mouse_entered.connect(on_card_mouse_movement)
	card.mouse_exited.connect(on_card_mouse_movement)
	card.angle = PI/2
	add_child(card)
	add_card(card)
	

func position_cards():
	for i in range(len(cards)):
		var angle = PI/2 - angle_between_cards * (i - (len(cards)-1) / 2.)
		var goal_position = Vector2(cos(angle) * big_radius, -sin(angle) * small_radius)
		var card_rotation = -small_radius**2 * goal_position.x / big_radius**2 / goal_position.y
		cards[i].tween_position(goal_position, atan(card_rotation), 0.5)
		get_tree().create_timer(0.25).timeout.connect(func(): cards[i].z_index=i)
		#cards[i].z_index = i

#func compute_goal_angles():
#	cards_goal_angles = []
#	for i in range(len(cards)):
#		cards_goal_angles.append()

func swap_cards(i, j):
	var cardi = cards[i]
	cards[i] = cards[j]
	cards[j] = cardi

func on_card_mouse_movement():
	var can_hover = true
	for i in range(len(cards)-1, -1, -1):
		if cards[i].is_hovered and can_hover:
			cards[i].tween_scale(1.1)
			can_hover = false
		else:
			cards[i].tween_scale(1.)
