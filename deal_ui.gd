extends Control
const CARD_SCENE = preload("res://elements/card.tscn")

var current_trick = null
var hand_to_deal = 0

var all_hands = []

func _ready():
	all_hands = [$Bottom/Hand, $Left/Hand, $Top/Hand, $Right/Hand]
	get_tree().create_timer(0.1).timeout.connect(_on_start_pressed)

func deal_card() -> void:
	var dealt_card = $Deck/Deck.cards.pop_back()
	all_hands[hand_to_deal%4].add_card(dealt_card, true)#, hand_to_deal%4 == 0)
	hand_to_deal += 1


func _on_start_pressed() -> void:
	$Deck/Deck.visible = true
	$Start.visible = false
	for i in range(20):
		get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 4. * i).timeout.connect(deal_card)
	get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION * 5).timeout.connect(start_trump_picker)

var trump_picker = null
func start_trump_picker():
	trump_picker = load("res://ui/trump_picker.tscn").instantiate()
	$Deck.add_child(trump_picker)
	var dealt_card = $Deck/Deck.cards.pop_back()
	trump_picker.add_card(dealt_card)
	trump_picker.no_trump.connect(func(): get_tree().quit())
	trump_picker.player_chose.connect(trump_card_selected)

func trump_card_selected(player, card, trump):
	$Deck/TrumpBackground.set_trump(trump)
	all_hands[player].add_card(card, false)
	for hand in all_hands:
		hand.trump = trump
	trump_picker.queue_free()
	hand_to_deal = player + 1
	for i in range(11):
		get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 4. * i).timeout.connect(deal_card)
	get_tree().create_timer(Hand.CARD_MOVEMENT_DURATION / 4. * 11 + Card.FLIP_DURATION).timeout.connect(start_round)

func start_round():
	var trick = load("res://elements/trick.tscn").instantiate()
	trick.hands = all_hands
	trick.trick_finished.connect(_on_trick_finished)
	$Deck.add_child(trick)

func _on_trick_finished(last_trick: int):
	for hand in all_hands:
		print("----------")
		for card in hand.cards_won:
			print(str(card.suit == hand.trump) + " " + str(card.rank))
	var team_points = $Bottom/Hand.points_won + $Top/Hand.points_won + (10 if last_trick else 0)
	var opponents_points = $Left/Hand.points_won + $Right/Hand.points_won + (10 if not last_trick else 0)
	$Label.text = str(team_points) + " - " + str(opponents_points) + "\n You " + ("Won" if team_points > opponents_points else "Lost") + "!"
	$Label.visible = true
	var card_cpt = 0
	for hand in all_hands:
		for card in hand.cards_won:
			get_tree().create_timer(card_cpt * Hand.CARD_MOVEMENT_DURATION / 2).timeout.connect(
				func(): $Deck/Deck.add_card(card))
			card_cpt += 1
	print(card_cpt)
	
