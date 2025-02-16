extends Node2D
class_name SimpleHand

signal play_card(card: Card)

@export var small_radius = 100
@export var big_radius = 200
@export var card_picker: CardPicker

const CARD_MOVEMENT_DURATION = 0.1
var angle_between_cards = PI / 12

var cards = []
var points_won = 0

func send_play_card_signal(card):
	card.highlighted = false
	for j in range(len(cards)):
		cards[j].highlighted = false
	card.tween_scale(1)
	play_card.emit(card)
	position_cards()


func _input(event):
	if event is InputEventMouseButton and event.pressed:
		print("pressed")
		for i in range(len(cards)-1, -1, -1):
			if cards[i].is_hovered:
				var selected_card = cards.pop_at(i)
				selected_card.mouse_movement.disconnect(on_card_mouse_movement)
				print("sent")
				card_picker.add_card(selected_card)
				position_cards()
				return

func add_card(card):
	ChildExchange.exchange(card, self)
	card.mouse_movement.connect(on_card_mouse_movement)
	card.is_hovered = false
	card.tween_scale(1.)
	cards.append(card)
	cards.sort_custom(Card.generate_order(null))
	position_cards()

func position_cards():
	for i in range(len(cards)):
		var angle = PI/2 - angle_between_cards * (i - (len(cards)-1) / 2.)
		var goal_position = Vector2(cos(angle) * big_radius, -sin(angle) * small_radius)
		var card_rotation = -small_radius**2 * goal_position.x / big_radius**2 / goal_position.y
		cards[i].tween_position(goal_position, atan(card_rotation), CARD_MOVEMENT_DURATION)
		get_tree().create_timer(CARD_MOVEMENT_DURATION / 2.).timeout.connect(func(): cards[i].z_index=i)

func on_card_mouse_movement():
	var can_hover = true
	for i in range(len(cards)-1, -1, -1):
		if cards[i].is_hovered and can_hover:
			cards[i].tween_scale(1.1)
			can_hover = false
		else:
			cards[i].tween_scale(1.)
